#include "IState.hpp"
#include "StateManager.hpp"
#include "Util.hpp"

#include <algorithm>

StateManager::StateManager()
{
}

void StateManager::PushState(std::unique_ptr<IState> a)
{
    m_statesToAdd.push_back(std::move(a));
}

const IState* StateManager::GetState() const
{
    if (m_states.empty())
        return nullptr;

    return m_states.back().get();
}

IState* StateManager::GetState()
{
    if (m_states.empty())
        return nullptr;

    return m_states.back().get();
}

void StateManager::PopState()
{
    m_statesToRemove.push_back(m_states.back().get());
}

void StateManager::BeginFrame()
{
    if (UNLIKELY(!m_statesToRemove.empty()))
    {
        for (auto& toRemove : m_statesToRemove)
        {
            auto it = std::find_if(std::begin(m_states), std::end(m_states), [toRemove](auto& ptr) { return ptr.get() == toRemove; });
            if (it != std::end(m_states))
                m_states.erase(it);
        }
        m_statesToRemove.clear();
    }
}

void StateManager::EndFrame()
{
    if (UNLIKELY(!m_statesToAdd.empty()))
    {
        for (auto& toAdd : m_statesToAdd)
        {
            m_states.push_back(std::move(toAdd));
        }
        m_statesToAdd.clear();
    }
}
