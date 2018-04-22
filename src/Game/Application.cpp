#include "Application.hpp"
#include "Inputs.hpp"
#include "IState.hpp"
#include "Math.hpp"
#include "Util.hpp"
#include "Version.hpp"

#include "States/GameState.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

Application* Application::s_singleton = nullptr;

Application::Application()
{
    s_singleton = this;
}

Application::~Application()
{
    s_singleton = nullptr;
}

bool Application::Init()
{
    auto& ver = Version::GetVersion();
    std::cout << "Starting..." << std::endl
              << "Version: " << ver.Major << "." << ver.Minor << "." << ver.Patch << " (" << ver.Revision << ")" << std::endl;

    std::string fonts[] = {
#if defined(_WIN32)
        "C:\\Windows\\Fonts\\consola.ttf"
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

    m_profiler.AddSections({ "Prepare", "State", "Finalize", "InputManager", "StateManager", "Events", "Overlay", "Draw" });

    m_inputMan.Prepare(Input_Count);
    m_inputMan[Input_Up].Bind = { InputManager::BindData::Bind_Keyboard, { { uint32_t(sf::Keyboard::Up), uint8_t(0) } } };
    m_inputMan[Input_Down].Bind = { InputManager::BindData::Bind_Keyboard, { { uint32_t(sf::Keyboard::Down), uint8_t(0) } } };
    m_inputMan[Input_Left].Bind = { InputManager::BindData::Bind_Keyboard, { { uint32_t(sf::Keyboard::Left), uint8_t(0) } } };
    m_inputMan[Input_Right].Bind = { InputManager::BindData::Bind_Keyboard, { { uint32_t(sf::Keyboard::Right), uint8_t(0) } } };
    m_inputMan[Input_Action].Bind = { InputManager::BindData::Bind_Keyboard, { { uint32_t(sf::Keyboard::Space), uint8_t(0) } } };
    m_inputMan[Input_Cancel].Bind = { InputManager::BindData::Bind_Keyboard, { { uint32_t(sf::Keyboard::Escape), uint8_t(0) } } };

    return true;
}

bool Application::Run()
{
    sf::Event ev;
    Version ver = Version::GetVersion();

    m_window.create(sf::VideoMode(800, 600), "LD41 " + ver.ToString(), sf::Style::Default);
    m_window.setFramerateLimit(1000);

    m_uiView = m_window.getDefaultView();
    m_gameView = sf::View({0, 0, 800, 600});

    m_stateMan.PushState<GameState>();

    unsigned int updates = 0,
                 ups = 0;
    float updateClock = 0;

    std::string debugOverlay;
    while (m_window.isOpen())
    {
        ++updates;
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

                        /*float ratio = uiSize.x / uiSize.x;
                        sf::Vector2f gameSize = m_gameView.getSize();
                        gameSize.x = gameSize.y * ratio;
                        m_gameView.setSize(gameSize);
                        m_gameView.setCenter(gameSize / 2.f);*/
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
            state->Draw(m_window);
            m_gameView = m_window.getView();

            m_window.setView(m_uiView);
            state->DrawUI(m_window);
        }
        m_profiler.EndSection("State"_profile);

        {
            sf::Text overlay(debugOverlay, m_defaultFont, 9u);

            m_window.setView(m_uiView);
            m_window.draw(overlay);
        }

        m_profiler.BeginSection("Finalize"_profile);
        m_profiler.BeginSection("StateManager"_profile);
        m_stateMan.EndFrame();
        m_profiler.EndSection("StateManager"_profile);

        m_profiler.BeginSection("Draw"_profile);
        m_window.display();
        m_profiler.EndSection("Draw"_profile);
        m_profiler.EndSection("Finalize"_profile);

        m_profiler.EndFrame();

        std::ostringstream oss;
        if (LIKELY(state))
            oss << "[" << std::string(state->GetName()) << "] @ " << ups << "UPS" << std::endl;
        oss << m_profiler.GetRoot();
        debugOverlay = oss.str();

        m_stateMan.SetFrameTime(std::chrono::duration_cast<std::chrono::microseconds>(m_profiler.GetRoot().TotalTime));

        updateClock += m_stateMan.GetFrameTimeDT();
        if (updateClock >= 1)
        {
            ups = updates;
            updateClock -= 1;
            updates = 0;
        }
    }

    return true;
}


sf::Font& Application::GetDefaultFont() { return m_defaultFont; }
const sf::Font& Application::GetDefaultFont() const { return m_defaultFont; }
sf::RenderTarget& Application::GetRenderTarget() { return m_window; }
const sf::RenderTarget& Application::GetRenderTarget() const { return m_window; }
sf::View& Application::GetGameView() { return m_gameView; }
const sf::View& Application::GetGameView() const { return m_gameView; }
sf::View& Application::GetUIView() { return m_uiView; }
const sf::View& Application::GetUIView() const { return m_uiView; }
InputManager& Application::GetInputManager() { return m_inputMan; }
const InputManager& Application::GetInputManager() const { return m_inputMan; }
StateManager& Application::GetStateManager() { return m_stateMan; }
const StateManager& Application::GetStateManager() const { return m_stateMan; }
Profiler& Application::GetProfiler() { return m_profiler; }
const Profiler& Application::GetProfiler() const { return m_profiler; }

const Application& Application::GetSingleton()
{
    return *s_singleton;
}
