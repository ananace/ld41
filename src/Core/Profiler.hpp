#pragma once

#include <chrono>
#include <functional>
#include <initializer_list>
#include <string>
#include <unordered_map>

class Profiler
{
public:
    struct ProfiledSection;
    class Iterator;

    Profiler();
    Profiler(const Profiler&) = delete;
    ~Profiler();

    Profiler& operator=(const Profiler&) = delete;

    static Profiler& GetSingleton();

    void AddSections(const std::initializer_list<std::string>& sections);
    void CleanupMemory();

    void BeginFrame();
    void EndFrame();

    void BeginSection(const std::string& str) { BeginSection(SectionHasher()(str)); }
    void EndSection(const std::string& str) { EndSection(SectionHasher()(str)); }

    void BeginSection(std::size_t strHash);
    void EndSection(std::size_t strHash);

    Iterator begin() const;
    Iterator end() const;

    const ProfiledSection& GetRoot() const;
    const std::string& GetSectionName(std::size_t strHash) const;

    void ForEach(const std::function<void(const ProfiledSection& section)>& iter) const;

    struct SectionHasher {
        std::size_t constexpr operator()(char const *input) const {
            return *input ?
                static_cast<unsigned int>(*input) + 33 * (*this)(input + 1) :
                5381;
        }
        std::size_t operator()(const std::string& str) const {
            return (*this)(str.c_str());
        }
    };

    class Section
    {
    public:
        Section(const std::string& name);
        Section(std::size_t hash);
        Section(const Section&) = delete;
        ~Section();

        Section& operator=(const Section&) = delete;

    private:
        std::size_t m_sectionHash;
    };

    struct ProfiledSection
    {
        typedef std::chrono::high_resolution_clock Clock;
        typedef Clock::time_point TimePoint;
        typedef Clock::duration Duration;

        ProfiledSection(const ProfiledSection& copy);
        ~ProfiledSection();

        TimePoint StartTime, EndTime;
        Duration MaxTime, MinTime, TotalTime;
        unsigned int CallCount;

        ProfiledSection *Parent, *Child, *Sibling;

        std::size_t SectionHash;

        Iterator begin() const;
        Iterator end() const;

        const std::string& GetName() const;
        std::string GetWholeName() const;
        Duration GetAvgTime() const;
        const ProfiledSection* GetChild(std::size_t hash) const;
        void ForEach(const std::function<void(const ProfiledSection& section)>& iter) const;

        bool IsRoot() const;

        friend std::ostream& operator<<(std::ostream& stream, const ProfiledSection& section);

    private:
        explicit ProfiledSection(std::size_t hash, ProfiledSection* parent = nullptr);

        ProfiledSection* GetOrCreateChild(std::size_t hash);

        void BeginCall();
        bool EndCall();
        void Reset();
        void CleanupMemory();

        unsigned int Recursion;
        bool Copied;

        friend class Profiler;
    };

    class Iterator
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef ProfiledSection value_type;
        typedef const ProfiledSection& reference;
        typedef const ProfiledSection* pointer;
        typedef std::forward_iterator_tag iterator_category;

        Iterator(const Iterator&) = default;
        Iterator(Iterator&&) = default;
        ~Iterator() = default;

        Iterator& operator=(const Iterator& rhs) = default;
        Iterator& operator=(Iterator&& rhs) = default;

        Iterator& operator++();
        Iterator& operator+=(int diff);
        Iterator operator++(int);

        bool operator==(const Iterator& rhs);
        bool operator!=(const Iterator& rhs);

        reference operator*() const;
        pointer operator->() const;

    private:
        explicit Iterator(const ProfiledSection* section, bool atEnd = false);

        const ProfiledSection *m_curChild, *m_curParent;

        friend class Profiler;
        friend struct Profiler::ProfiledSection;
    };

private:
    static Profiler* g_profiler;

    ProfiledSection m_rootNode;
    ProfiledSection* m_curNode;
    std::unordered_map<std::size_t, std::string> m_sectionNames;
};

std::size_t constexpr operator "" _profile(const char* str, size_t) {
  return Profiler::SectionHasher()(str);
}

