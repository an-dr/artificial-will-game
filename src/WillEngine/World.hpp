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
#include <entt/entt.hpp>

namespace will_engine {
    class World {
        entt::registry registry_; // All entities/components here

    public:
        World() {
            auto entity = registry_.create();
            registry_.emplace<int>(entity, 1);
        }

        // Systems
        void update_movement(float dt);

        void update_animation(float dt);

        void render(SDL_Renderer *renderer);
    };
}
