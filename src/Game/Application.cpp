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

    m_profiler.AddSections({ "Prepare", "Finalize", "InputManager", "StateManager", "Timesink" });

    return true;
}

bool Application::Run()
{
    int it = 0;
    std::chrono::microseconds count = {};

    while (true)
    {
        ++it;
        m_profiler.BeginFrame();

        m_profiler.BeginSection("Prepare"_profile);
        m_profiler.BeginSection("StateManager"_profile);
        m_stateMan.BeginFrame();
        m_profiler.EndSection("StateManager"_profile);

        m_profiler.BeginSection("InputManager"_profile);
        m_inputMan.Update();
        m_profiler.EndSection("InputManager"_profile);
        m_profiler.EndSection("Prepare"_profile);

        auto* state = m_stateMan.GetState();
        if (LIKELY(state))
        {
            // state->Update(); etc
        }

        m_profiler.BeginSection("Timesink"_profile);
        for (int i = 0; i < 1000000; ++i)
            (void)i;
        m_profiler.EndSection("Timesink"_profile);


        m_profiler.BeginSection("Finalize"_profile);
        m_profiler.BeginSection("StateManager"_profile);
        m_stateMan.EndFrame();
        m_profiler.EndSection("StateManager"_profile);
        m_profiler.EndSection("Finalize"_profile);

        m_profiler.EndFrame();

        count += std::chrono::duration_cast<std::chrono::microseconds>(m_profiler.GetRoot().TotalTime);

        if (count.count() > 1000000)
        {
            std::cout << "~" << (1000000 / std::chrono::duration_cast<std::chrono::microseconds>(m_profiler.GetRoot().TotalTime).count()) << " UPS (" << it << ")" << std::endl << m_profiler.GetRoot() << std::endl;
            count = {};
            it = 0;
        }
    }

    return true;
}
