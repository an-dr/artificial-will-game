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
#include "../entity_components/ComponentGeometry.hpp"
#include "../entity_components/ComponentPlayer.hpp"
#include "BaseSystem.hpp"

namespace will_engine {


class Input : public BaseSystem {

public:
    Input() = default;

    auto process() -> void {

        const Uint8 *keyState = SDL_GetKeyboardState(nullptr);
        bool movingLeft = keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT];
        bool movingRight = keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT];
        bool movingUp = keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP];
        bool movingDown = keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN];

        constexpr int d_koef = 2;

        int dx = 0;
        int dy = 0;
        if (movingLeft)
            dx -= d_koef;
        if (movingRight)
            dx += d_koef;
        if (movingUp)
            dy -= d_koef;
        if (movingDown)
            dy += d_koef;

        auto players = getRegistry()->view<ComponentPlayer>();

        for (auto entity : players) {
            auto p_geo = getRegistry()->try_get<ComponentGeometry>(entity);
            if (!p_geo) {
                throw std::runtime_error("No geometry for this entity");
            }
            p_geo->x += dx;
            p_geo->y += dy;
        }
    }
};
}  // namespace will_engine