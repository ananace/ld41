#pragma once

#include <chrono>
#include <memory>
#include <vector>

class IState;

class StateManager
{
public:
    StateManager();
    StateManager(const StateManager&) = delete;
    ~StateManager() = default;

    StateManager& operator=(const StateManager&) = delete;

    template<typename T, typename... Args>
    void PushState(Args&&... args);
    void PushState(std::unique_ptr<IState> a);
    const IState* GetState() const;
    IState* GetState();
    void PopState();

    void BeginFrame();
    void EndFrame();

    void SetFrameTime(float dt);
    void SetFrameTime(const std::chrono::microseconds& us);
    const std::chrono::microseconds& GetFrameTime() const noexcept;
    float GetFrameTimeDT() const;

private:
    std::vector<std::unique_ptr<IState>> m_states;

    // For delayed state sync
    std::vector<std::unique_ptr<IState>> m_statesToAdd;
    std::vector<IState*> m_statesToRemove;

    // State tracking
    std::chrono::microseconds m_lastFrameTime;
};

#include "StateManager.inl"
