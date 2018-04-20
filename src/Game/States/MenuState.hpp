#pragma once

#include <IState.hpp>

class MenuState : public IState
{
public:
    MenuState();
    ~MenuState();

    void Update();

    void Draw(sf::RenderTarget&) const;
    void DrawUI(sf::RenderTarget&) const;

    const char* GetName() const { return "MenuState"; }
private:
};
