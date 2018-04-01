#include "Profiler.hpp"

#include <iomanip>
#include <iostream>
#include <list>

#include <cassert>

Profiler* Profiler::g_profiler = nullptr;

Profiler& Profiler::GetSingleton()
{
    assert(g_profiler);
    return *g_profiler;
}

Profiler::Profiler()
    : m_rootNode("root"_profile)
{
    g_profiler = this;

    AddSections({"root"});
}

Profiler::~Profiler()
{
    g_profiler = nullptr;
}

void Profiler::AddSections(const std::initializer_list<std::string>& sections)
{
    auto hasher = SectionHasher();
    for (auto& str : sections)
        m_sectionNames[hasher(str)] = str;
}
void Profiler::CleanupMemory()
{
    m_rootNode.CleanupMemory();
}

void Profiler::BeginFrame()
{
    m_rootNode.Reset();
    m_curNode = &m_rootNode;

    m_rootNode.BeginCall();
}
void Profiler::EndFrame()
{
    m_rootNode.EndCall();
}

void Profiler::BeginSection(std::size_t strHash)
{
    if (strHash != m_curNode->SectionHash)
        m_curNode = m_curNode->GetOrCreateChild(strHash);
    m_curNode->BeginCall();
}
void Profiler::EndSection(std::size_t strHash)
{
    assert(strHash == m_curNode->SectionHash);

    if (m_curNode->EndCall())
        if (!m_curNode->IsRoot())
            m_curNode = m_curNode->Parent;
}

Profiler::Iterator Profiler::begin() const
{
    return m_rootNode.begin();
}
Profiler::Iterator Profiler::end() const
{
    return m_rootNode.end();
}

const Profiler::ProfiledSection& Profiler::GetRoot() const
{
    return m_rootNode;
}

const std::string& Profiler::GetSectionName(std::size_t strHash) const
{
    return m_sectionNames.at(strHash);
}

void Profiler::ForEach(const std::function<void(const ProfiledSection& section)>& iter) const
{
    m_rootNode.ForEach(iter);
}

Profiler::Section::Section(const std::string& name)
    : m_sectionHash(Profiler::SectionHasher()(name))
{
    Profiler::GetSingleton().BeginSection(m_sectionHash);
}

Profiler::Section::Section(std::size_t hash)
    : m_sectionHash(hash)
{
    Profiler::GetSingleton().BeginSection(m_sectionHash);
}

Profiler::Section::~Section()
{
    Profiler::GetSingleton().EndSection(m_sectionHash);
}

Profiler::ProfiledSection::ProfiledSection(std::size_t hash, ProfiledSection* parent)
    : StartTime(TimePoint {})
    , EndTime(TimePoint {})
    , MaxTime(Duration {})
    , MinTime(Duration {})
    , TotalTime(Duration {})
    , CallCount(0)
    , Parent(parent)
    , Child(nullptr)
    , Sibling(nullptr)
    , SectionHash(hash)
    , Recursion(0)
    , Copied(false)
{
}

Profiler::ProfiledSection::ProfiledSection(const ProfiledSection& copy)
    : StartTime(copy.StartTime)
    , EndTime(copy.EndTime)
    , MaxTime(copy.MaxTime)
    , MinTime(copy.MinTime)
    , TotalTime(copy.TotalTime)
    , CallCount(copy.CallCount)
    , Parent(copy.Parent)
    , Child(copy.Child)
    , Sibling(copy.Sibling)
    , SectionHash(copy.SectionHash)
    , Recursion(copy.Recursion)
    , Copied(true)
{

}

Profiler::ProfiledSection::~ProfiledSection()
{
    if (!Copied)
        CleanupMemory();
}

Profiler::Iterator Profiler::ProfiledSection::begin() const
{
    return Iterator(this);
}
Profiler::Iterator Profiler::ProfiledSection::end() const
{
    return Iterator(this, true);
}

const std::string& Profiler::ProfiledSection::GetName() const
{
    return Profiler::GetSingleton().GetSectionName(SectionHash);
}

std::string Profiler::ProfiledSection::GetWholeName() const
{
    std::string ret = "";
    if (Parent && !Parent->IsRoot())
        ret = Parent->GetWholeName() + "::";
    return ret + GetName();
}

Profiler::ProfiledSection::Duration Profiler::ProfiledSection::GetAvgTime() const
{
    return TotalTime / CallCount;
}

const Profiler::ProfiledSection* Profiler::ProfiledSection::GetChild(std::size_t hash) const
{
    if (!Child)
        return nullptr;

    const ProfiledSection *it = Child;
    do
    {
        if (it->SectionHash == hash)
            return it;
    }
    while ((it = it->Sibling) != nullptr);

    return nullptr;
}

void Profiler::ProfiledSection::ForEach(const std::function<void(const ProfiledSection& section)>& iter) const
{
    std::cerr << "ForEach: " << GetName() << " = [" << Parent << ", " << Child << ", " << Sibling << "]" << std::endl;
    std::list<std::pair<Iterator, Iterator>> iterators;
    iterators.push_back(std::make_pair(begin(), end()));

    while (!iterators.empty())
    {
        auto& cur = iterators.back();
        if (cur.first == cur.second)
        {
            iterators.pop_back();
            continue;
        }

        auto& it = cur.first;

        iter(*it);

        if (it->Child)
            iterators.push_back(std::make_pair(it->Child->begin(), it->Child->end()));

        ++it;
    }
}

bool Profiler::ProfiledSection::IsRoot() const
{
    return Parent == nullptr;
}

std::ostream& operator<<(std::ostream& stream, const Profiler::ProfiledSection& section)
{
    int level = 0;
    std::function<void(const Profiler::ProfiledSection&)> lambda;
    lambda = [&lambda, &level, &stream, section](const Profiler::ProfiledSection& it) {
        std::cerr << "<<: " << it.GetName() << std::endl;
        std::string indent = "";
        for (int i = 0; i < level; ++i)
            indent += "  ";

        stream << indent << it.GetName() << ":";
        if (it.CallCount > 0)
        {
            float avg = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(it.GetAvgTime()).count();
            stream << " " << std::setprecision(2) << std::fixed << avg << "ms";
        }
        if (it.CallCount > 1)
            stream << " * " << it.CallCount;

        if (level > 0)
        {
            float perc = (it.TotalTime.count() / float(it.Parent->TotalTime.count()));
            if (perc > 0.05f)
                stream << " (" << perc << "%)";
        }

        stream << std::endl;

        if (it.Child && it.Child->CallCount > 0)
        {
            ++level;
            it.ForEach(lambda);
            --level;
        }
    };

    section.ForEach(lambda);

    return stream;
}

Profiler::ProfiledSection* Profiler::ProfiledSection::GetOrCreateChild(std::size_t hash)
{
    if (Child == nullptr)
    {
        auto* section = new ProfiledSection(hash, this);
        Child = section;
        return section;
    }

    ProfiledSection *it = Child, *old = nullptr;
    do
    {
        old = it;

        if (it->SectionHash == hash)
            return it;
    }
    while ((it = it->Sibling) != nullptr);

    auto* section = new ProfiledSection(hash, this);
    old->Sibling = section;
    return section;
}

void Profiler::ProfiledSection::BeginCall()
{
    ++CallCount;
    if (Recursion++ == 0)
        StartTime = Clock::now();
}

bool Profiler::ProfiledSection::EndCall()
{
    if (--Recursion == 0)
    {
        EndTime = Clock::now();
        auto diff = EndTime - StartTime;
        if (MinTime == Duration{} || MinTime > diff)
            MinTime = diff;
        if (MaxTime < diff)
            MaxTime = diff;
        TotalTime += diff;
    }

    return Recursion == 0;
}

void Profiler::ProfiledSection::Reset()
{
    CallCount = Recursion = 0;
    StartTime = EndTime = {};
    MinTime = MaxTime = TotalTime = {};

    if (Child != nullptr)
        Child->Reset();
    if (Sibling != nullptr)
        Sibling->Reset();
}

void Profiler::ProfiledSection::CleanupMemory()
{
    if (Child != nullptr)
        delete Child;
    if (Sibling != nullptr)
        delete Sibling;

    Child = Sibling = nullptr;
}

Profiler::Iterator::Iterator(const ProfiledSection* section, bool atEnd)
    : m_curChild((!atEnd && section) ? section->Child : nullptr)
    , m_curParent(section)
{
}

Profiler::Iterator& Profiler::Iterator::operator++()
{
    if (m_curChild != nullptr)
        m_curChild = m_curChild->Sibling;
    return *this;
}

Profiler::Iterator& Profiler::Iterator::operator+=(int diff)
{
    while (m_curChild != nullptr && diff-- > 0)
        m_curChild = m_curChild->Sibling;
    return *this;
}

Profiler::Iterator Profiler::Iterator::operator++(int)
{
    Iterator ret = *this;
    ++(*this);
    return ret;
}

bool Profiler::Iterator::operator==(const Iterator& rhs)
{
    return m_curChild == rhs.m_curChild
        && m_curParent == rhs.m_curParent;
}
bool Profiler::Iterator::operator!=(const Iterator& rhs)
{
    return m_curChild != rhs.m_curChild
        || m_curParent != rhs.m_curParent;
}

Profiler::Iterator::reference Profiler::Iterator::operator*() const
{
    return *m_curChild;
}
Profiler::Iterator::pointer Profiler::Iterator::operator->() const
{
    return m_curChild;
}
