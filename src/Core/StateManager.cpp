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

void StateManager::SetFrameTime(float dt)
{
	m_lastFrameTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<float>(dt));
}
void StateManager::SetFrameTime(const std::chrono::microseconds& us)
{
	m_lastFrameTime = us;
}
const std::chrono::microseconds& StateManager::GetFrameTime() const noexcept
{
	return m_lastFrameTime;
}
float StateManager::GetFrameTimeDT() const
{
    return std::chrono::duration_cast<std::chrono::duration<float>>(m_lastFrameTime).count();
}
