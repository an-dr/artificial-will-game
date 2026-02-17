// *************************************************************************
//
// Copyright (c) 2026 Andrei Gramakov. All rights reserved.
//
// This file is licensed under the terms of the MIT license.
// For a copy, see: https://opensource.org/licenses/MIT
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#pragma once
#include <SDL.h>
#include <optional>

namespace will_engine
{
    enum class InputEvent
    {
        None,
        Up,
        Down,
        Left,
        Right,
    };

    class Input
    {
    public:
        Input() = default;

        auto get() -> std::optional<InputEvent>
        {
            const Uint8* keyState = SDL_GetKeyboardState(nullptr);

            bool movingLeft = keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT];
            bool movingRight = keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT];
            bool movingUp = keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP];
            bool movingDown = keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN];

            InputEvent event = InputEvent::None;
            if (movingDown)
            {
                event = InputEvent::Down;
            }
            else if (movingLeft)
            {
                event = InputEvent::Left;
            }
            else if (movingRight)
            {
                event = InputEvent::Right;
            }
            else if (movingUp)
            {
                event = InputEvent::Up;
            }
            else
            {
                event = InputEvent::None;
            }
            return event;
        }
    };
} // engine
