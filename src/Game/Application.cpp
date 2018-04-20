#include "Application.hpp"
#include "IState.hpp"
#include "Math.hpp"
#include "Util.hpp"
#include "Version.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

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

    std::string fonts[] = {
#if defined(_WIN32)
#else
        "/usr/share/fonts/corefonts/andalemo.ttf",
        "/usr/share/fonts/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/liberation-fonts/LiberationMono-Regular.ttf",
        "/usr/share/fonts/noto/NotoMono-Regular.ttf"
#endif
    };

    auto it = std::find_if(std::begin(fonts), std::end(fonts), [=](const std::string& file) { return m_defaultFont.loadFromFile(file); });
    if (it == std::end(fonts))
        std::cout << "No default font located, this could cause some issues, sorry." << std::endl;
    else
        std::cout << "Loaded default font; " << *it << std::endl;

    m_profiler.AddSections({ "Prepare", "State", "Finalize", "InputManager", "StateManager", "Events" });

    return true;
}

bool Application::Run()
{
    sf::Event ev;
    Version ver = Version::GetVersion();

    m_window.create(sf::VideoMode(800, 600), "LD41 " + ver.ToString(), sf::Style::Default);

    while (m_window.isOpen())
    {
        m_profiler.BeginFrame();

        m_window.clear();

        m_profiler.BeginSection("Prepare"_profile);
        m_profiler.BeginSection("StateManager"_profile);
        m_stateMan.BeginFrame();
        m_profiler.EndSection("StateManager"_profile);

        auto* state = m_stateMan.GetState();

        m_profiler.BeginSection("InputManager"_profile);
        m_inputMan.Update();
        m_profiler.EndSection("InputManager"_profile);

        m_profiler.BeginSection("Events"_profile);
        while (m_window.pollEvent(ev))
        {
            bool handled = false;

            m_profiler.BeginSection("State"_profile);
            if (LIKELY(state))
                handled = state->Event(ev);
            m_profiler.EndSection("State"_profile);

            if (!handled)
                switch (ev.type)
                {
                case sf::Event::Resized: {
                        sf::Vector2f uiSize(ev.size.width, ev.size.height);
                        m_uiView.setSize(uiSize);
                        m_uiView.setCenter(uiSize / 2.f);

                        float ratio = uiSize.x / uiSize.y;

                        sf::Vector2f gameSize = m_gameView.getSize();
                        gameSize.x = gameSize.y * ratio;
                        m_gameView.setSize(gameSize);
                    }; break;

                case sf::Event::Closed: m_window.close(); break;

                default: break;
                }
        }
        m_profiler.EndSection("Events"_profile);
        m_profiler.EndSection("Prepare"_profile);

        m_profiler.BeginSection("State"_profile);
        if (LIKELY(state))
        {
            state->Update();

            m_window.setView(m_gameView);
            state->Draw();
            m_gameView = m_window.getView();

            m_window.setView(m_uiView);
            state->DrawUI();
        }
        m_profiler.EndSection("State"_profile);

        m_profiler.BeginSection("Finalize"_profile);
        m_profiler.BeginSection("StateManager"_profile);
        m_stateMan.EndFrame();
        m_profiler.EndSection("StateManager"_profile);
        m_profiler.EndSection("Finalize"_profile);

        m_profiler.EndFrame();

        {
            std::ostringstream oss;
            oss << m_profiler.GetRoot();
            sf::Text overlay(oss.str(), m_defaultFont, 9u);

            m_window.setView(m_uiView);
            m_window.draw(overlay);
        }

        m_window.display();

        m_stateMan.SetFrameTime(std::chrono::duration_cast<std::chrono::microseconds>(m_profiler.GetRoot().TotalTime));
    }

    return true;
}
