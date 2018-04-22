#pragma once

namespace sf { class Event; class RenderTarget; }

class StateManager;

class IState
{
public:
    IState()
        : m_stateManager(nullptr)
    {
    }
    virtual ~IState()
    {
        m_stateManager = nullptr;
    }

    inline bool IsAttached() const { return bool(m_stateManager); }

    virtual void Update() = 0;
    virtual bool Event(sf::Event&) { return false; }

    virtual void Draw(sf::RenderTarget&) const = 0;
    virtual void DrawUI(sf::RenderTarget&) const = 0;

    virtual const char* GetName() const = 0;

protected:
    inline const StateManager& GetStateManager() const { return *m_stateManager; }
    StateManager& GetStateManager() { return *m_stateManager; }

private:
    StateManager* m_stateManager;

    friend class StateManager;
};
