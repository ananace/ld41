#pragma once

#include <vector>

#include <cstdint>

class InputManager
{
public:
    struct Input;

    InputManager();
    InputManager(InputManager&&) = default;
    InputManager(const InputManager&) = delete;
    ~InputManager() = default;

    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = default;

    void Prepare(std::size_t inputCount);
    void Update();

    Input& operator[](std::size_t inp);
    const Input& operator[](std::size_t inp) const;

    struct BindData
    {
        enum Modifiers {
            Mod_LCtrl  = 1 << 0,
            Mod_LAlt   = 1 << 1,
            Mod_LShift = 1 << 2,
            Mod_LSuper = 1 << 3,

            Mod_RCtrl  = 1 << 4,
            Mod_RAlt   = 1 << 5,
            Mod_RShift = 1 << 6,
            Mod_RSuper = 1 << 7,

            Mod_Menu   = 1 << 8,

            Mod_Ctrl   = Mod_LCtrl  | Mod_RCtrl,
            Mod_Alt    = Mod_LAlt   | Mod_RAlt,
            Mod_Shift  = Mod_LShift | Mod_RShift,
            Mod_Super  = Mod_LSuper | Mod_RSuper,
        };

        enum BindType {
            Bind_Invalid,

            Bind_Keyboard,
            Bind_MButton,
            Bind_JAxis,
            Bind_JButton,
        } Type;

        union
        {
            struct {
                int32_t Key;
                uint8_t Modifiers;
            } Keyboard;

            struct {
                int32_t Button;
            } MButton;

            struct {
                int32_t Joystick;
                int32_t Button;
            } JButton;

            struct {
                int32_t Joystick;
                int32_t Axis;
                bool Negative;
            } JAxis;
        };
    };

    struct Input
    {
        BindData Bind;
        float CurrentValue,
              LastValue;

        inline bool IsPressStart() const { return IsPressed() && !WasPressed(); }
        inline bool IsPressHold() const { return IsPressed() && WasPressed(); }
        inline bool IsPressStop() const { return IsReleased() && !WasReleased(); }

        inline bool IsPressed() const { return CurrentValue >= 0.55f; }
        inline bool IsReleased() const { return CurrentValue < 0.45f; }
        inline bool WasPressed() const { return LastValue >= 0.55f; }
        inline bool WasReleased() const { return LastValue < 0.45f; }
    };

private:

    std::vector<Input> m_inputs;
};
