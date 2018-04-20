#include "StateManager.hpp"

template<typename T, typename... Args>
void StateManager::PushState(Args&&... args)
{
    PushState(std::make_unique<T>(std::forward<Args>(args)...));
}
