#include "InputManager.hpp"

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

InputManager::InputManager()
    : m_inputs(0)
{
}

void InputManager::Prepare(std::size_t inputCount)
{
    m_inputs.resize(inputCount);
}

void InputManager::Update()
{
    for (auto& input : m_inputs)
    {
        if (input.Bind.Type == BindData::Bind_Invalid)
            continue;

        input.LastValue = input.CurrentValue;
        switch (input.Bind.Type)
        {
        case BindData::Bind_Keyboard:
            {
                auto& key = input.Bind.Keyboard;

                bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key.Key));
                uint8_t modifiersPressed = 0;

                if ((key.Modifiers & BindData::Mod_Ctrl) == BindData::Mod_Ctrl)
                    modifiersPressed |= (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                                         || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                        ? BindData::Mod_Ctrl : 0;
                else if ((key.Modifiers & BindData::Mod_Ctrl) != 0)
                    modifiersPressed |= ((key.Modifiers & BindData::Mod_LCtrl)
                                         ? sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                                         : sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                        ? BindData::Mod_Ctrl : 0;

                if ((key.Modifiers & BindData::Mod_Ctrl) == BindData::Mod_Ctrl)
                    modifiersPressed |= (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                                         || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                        ? BindData::Mod_Ctrl : 0;
                else if ((key.Modifiers & BindData::Mod_Ctrl) != 0)
                    modifiersPressed |= ((key.Modifiers & BindData::Mod_LCtrl)
                                         ? sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                                         : sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                        ? BindData::Mod_Ctrl : 0;

                if ((key.Modifiers & BindData::Mod_Ctrl) == BindData::Mod_Ctrl)
                    modifiersPressed |= (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                                         || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                        ? BindData::Mod_Ctrl : 0;
                else if ((key.Modifiers & BindData::Mod_Ctrl) != 0)
                    modifiersPressed |= ((key.Modifiers & BindData::Mod_LCtrl)
                                         ? sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                                         : sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                        ? BindData::Mod_Ctrl : 0;

                if ((key.Modifiers & BindData::Mod_Ctrl) == BindData::Mod_Ctrl)
                    modifiersPressed |= (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                                         || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                        ? BindData::Mod_Ctrl : 0;
                else if ((key.Modifiers & BindData::Mod_Ctrl) != 0)
                    modifiersPressed |= ((key.Modifiers & BindData::Mod_LCtrl)
                                         ? sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                                         : sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                        ? BindData::Mod_Ctrl : 0;

                if ((key.Modifiers & BindData::Mod_Menu) == BindData::Mod_Menu)
                    modifiersPressed |= sf::Keyboard::isKeyPressed(sf::Keyboard::Menu)
                        ? BindData::Mod_Menu : 0;

                input.CurrentValue = float(pressed && ((key.Modifiers & modifiersPressed) == key.Modifiers));
            } break;
        case BindData::Bind_MButton:
            {
                auto& button = input.Bind.MButton;

                input.CurrentValue = sf::Mouse::isButtonPressed(sf::Mouse::Button(button.Button));
            } break;
        case BindData::Bind_JButton:
            {
                auto& button = input.Bind.JButton;

                input.CurrentValue = sf::Joystick::isButtonPressed(button.Joystick, button.Button);
            } break;
        case BindData::Bind_JAxis:
            {
                auto& axis = input.Bind.JAxis;

                float value = sf::Joystick::getAxisPosition(axis.Joystick, sf::Joystick::Axis(axis.Axis));
                bool wasNeg = value < 0;
                value = abs(value);

                if (axis.Negative == wasNeg)
                    input.CurrentValue = value / 100.f;
            } break;

        default:
            // Error?
            break;
        }
    }
}
