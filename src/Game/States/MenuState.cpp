#include "MenuState.hpp"
#include "GameState.hpp"

#include <Application.hpp>
#include <Inputs.hpp>
#include <StateManager.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

MenuState::MenuState()
{
    m_background.loadFromFile("resources/MenuBackground.png");
}
MenuState::~MenuState()
{

}

void MenuState::Update()
{
    auto& inp = Application::GetSingleton().GetInputManager();

    if (inp[Input_Action].IsPressStop())
    {
        GetStateManager().PopState();
        GetStateManager().PushState<GameState>();
    }
}

void MenuState::Draw(sf::RenderTarget&) const {}
void MenuState::DrawUI(sf::RenderTarget& rt) const
{
    sf::Sprite background(m_background);

    float scale = std::max(rt.getView().getSize().y / m_background.getSize().y, rt.getView().getSize().x / m_background.getSize().x);
    background.setScale(scale, scale);
    background.setPosition(rt.getView().getCenter());
    background.setOrigin(sf::Vector2f(m_background.getSize()) / 2.f);

    rt.draw(background);

    sf::Text title("AsteroidSweeper xTreme", Application::GetSingleton().GetDefaultFont());
    title.setCharacterSize(38);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2.5f);
    title.setRotation(-29.f);

    auto rect = title.getLocalBounds();
    title.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
    title.setPosition(rt.getView().getCenter());
    rt.draw(title);

    sf::Text anyKey("< Press Space To Start >", Application::GetSingleton().GetDefaultFont());
    anyKey.setCharacterSize(18);
    anyKey.setOutlineColor(sf::Color::Black);
    anyKey.setOutlineThickness(1.5f);

    rect = anyKey.getLocalBounds();
    anyKey.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
    anyKey.setPosition({ rt.getView().getCenter().x, anyKey.getOrigin().y + 15.f });

    rt.draw(anyKey);

    anyKey.setPosition({ rt.getView().getCenter().x, rt.getView().getSize().y - anyKey.getOrigin().y - 15.f });
    anyKey.setOutlineColor(sf::Color::White);
    anyKey.setFillColor(sf::Color::Black);
    rt.draw(anyKey);

    sf::Text sweeperControls("    Arrow keys to move,\ntap space to reveal a square,\n  long tap space to flag it.\n\n    Don't hit the mines.", Application::GetSingleton().GetDefaultFont());
    sweeperControls.setRotation(-29.f);
    sweeperControls.setOutlineColor(sf::Color::White);
    sweeperControls.setOutlineThickness(2.5f);
    sweeperControls.setFillColor(sf::Color::Black);
    rect = sweeperControls.getLocalBounds();
    sweeperControls.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
    sweeperControls.setPosition({ rt.getView().getCenter().x + rt.getView().getSize().x / 6.f, rt.getView().getSize().y / 1.65f });
    rt.draw(sweeperControls);

    sf::Text asteroidsControls("Arrow keys to move,\n  space to fire.\n\nAvoid the asteroids.", Application::GetSingleton().GetDefaultFont());
    asteroidsControls.setRotation(-29.f);
    asteroidsControls.setOutlineColor(sf::Color::Black);
    asteroidsControls.setOutlineThickness(2.5f);
    asteroidsControls.setFillColor(sf::Color::White);
    rect = asteroidsControls.getLocalBounds();
    asteroidsControls.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
    asteroidsControls.setPosition({ rt.getView().getCenter().x - rt.getView().getSize().x / 6.f, rt.getView().getSize().y / 2.65f });
    rt.draw(asteroidsControls);
}
