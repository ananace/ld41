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

    /** Create and push a new state.
     *
     * \tparam The state class to push
     * \param args The constructor arguments to use 
     *
     * \note The state will be pushed onto the stack at the end of the frame.
     *
     * \sa PushState(std::unique_ptr<IState>)
     * \sa PopState()
     */
    template<typename T, typename... Args>
    void PushState(Args&&... args);
    /** Push a pre-created state.
     * 
     * \param state The state to push onto the stack,
     *              memory management will be claimed by the State Manager.
     *
     * \note The state will be pushed onto the stack at the end of the frame.
     *
     * \sa PushState(Args&&...)
     * \sa PopState()
     */
    void PushState(std::unique_ptr<IState> state);
    const IState* GetState() const;
    IState* GetState();
    /** Pop the current state.
     *
     * \note The clearing of memory will happen at the beginning of the next frame.
     *
     * \sa PushState(std::unique_ptr<IState>)
     * \sa PushState(Args&&...)
     */
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
