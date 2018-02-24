#include "Application.hpp"
#include "IState.hpp"
#include "Util.hpp"
#include "Version.hpp"

#include <iostream>

Application::Application()
{
}

Application::~Application()
{
}

bool Application::Init()
{
    auto& ver = Version::GetVersion();
    std::cout << "Starting..." << std::endl
        << "Version: " << ver.Major << "." << ver.Minor << "." << ver.Patch << " (" << ver.Revision << ")" << std::endl;

    return true;
}

bool Application::Run()
{
    while (true)
    {
        m_stateMan.BeginFrame();

        auto* state = m_stateMan.GetState();
        if (LIKELY(state))
        {
            // state->Update(); etc
        }

        m_stateMan.EndFrame();
    }

    return true;
}
