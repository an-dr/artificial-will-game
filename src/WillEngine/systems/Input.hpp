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
#include <cute_c2.h>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include "../entity_components/ComponentInput.hpp"
#include "../entity_components/ComponentPlayer.hpp"
#include "BaseSystem.hpp"

namespace will_engine {

class Input : public BaseSystem {

    static auto get_direction([[maybe_unused]] unsigned int player_id) -> glm::vec2 {
        const Uint8 *key_state = SDL_GetKeyboardState(nullptr);

        glm::vec2 direction{0.0f, 0.0f};
        if (key_state[SDL_SCANCODE_A] || key_state[SDL_SCANCODE_LEFT])
            direction.x -= 1.0f;
        if (key_state[SDL_SCANCODE_D] || key_state[SDL_SCANCODE_RIGHT])
            direction.x += 1.0f;
        if (key_state[SDL_SCANCODE_W] || key_state[SDL_SCANCODE_UP])
            direction.y -= 1.0f;
        if (key_state[SDL_SCANCODE_S] || key_state[SDL_SCANCODE_DOWN])
            direction.y += 1.0f;
        if (glm::length(direction) > 0.0f)
            direction = glm::normalize(direction);
        return direction;
    }

public:
    Input() = default;

    auto process(float delta_time) const -> void {
        auto reg = getRegistry();
        auto players = reg->view<ComponentPlayer>();

        for (auto player_entity : players) {
            auto &player = players.get<ComponentPlayer>(player_entity);

            auto direction = get_direction(player.player_id);
            auto input = &reg->get<ComponentInput>(player_entity);
            input->input_keyboard = direction;
        }
    }
};

}  // namespace will_engine