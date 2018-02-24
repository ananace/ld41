#pragma once

class StateManager;

class IState
{
public:
    IState()
        : m_stateManager(nullptr)
    {
    }
    virtual ~IState() {}

    inline bool IsAttached() const { return bool(m_stateManager); }

protected:
    inline const StateManager& GetStateManager() const { return *m_stateManager; }
    StateManager& GetStateManager() { return *m_stateManager; }

private:
    StateManager* m_stateManager;

    friend class StateManager;
};
