#pragma once

#include "Profiler.hpp"
#include "InputManager.hpp"
#include "StateManager.hpp"

class Application
{
public:
    Application();
    Application(const Application&) = delete;
    ~Application();

    Application& operator=(const Application&) = delete;

    bool Init();
    bool Run();

private:
    InputManager m_inputMan;
    Profiler m_profiler;
    StateManager m_stateMan;
};
