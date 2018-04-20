#pragma once

#include "Profiler.hpp"
#include "InputManager.hpp"
#include "StateManager.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

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

    sf::Font m_defaultFont;
    sf::RenderWindow m_window;
    sf::View m_gameView, m_uiView;
};
