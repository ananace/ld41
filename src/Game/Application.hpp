#pragma once

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
    StateManager m_stateMan;
};
