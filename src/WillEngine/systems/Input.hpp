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
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include "../entity_components/ComponentGeometry.hpp"
#include "../entity_components/ComponentPhysical.hpp"
#include "../entity_components/ComponentPlayer.hpp"
#include "BaseSystem.hpp"

namespace will_engine {


class Input : public BaseSystem {

    b2WorldId physicsWorld_;

public:
    Input() = default;

    auto setPhysicsWorld(b2WorldId physicsWorld) { physicsWorld_ = physicsWorld; }

    auto process(float dt, float ppm) const -> void {
        const Uint8 *keyState = SDL_GetKeyboardState(nullptr);

        glm::vec2 direction{0.0f, 0.0f};
        if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])  direction.x -= 1.0f;
        if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) direction.x += 1.0f;
        if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])    direction.y -= 1.0f;
        if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN])  direction.y += 1.0f;
        if (glm::length(direction) > 0.0f) direction = glm::normalize(direction);

        // Set desired velocity — Box2D's solver handles all collision response
        auto players = getRegistry()->view<ComponentPlayer, ComponentPhysical>();
        for (auto entity : players) {
            auto &physics = players.get<ComponentPhysical>(entity);
            constexpr float speed = 5.0f;
            b2Body_SetLinearVelocity(physics.body_id,
                                     {direction.x * speed, direction.y * speed});
        }

        b2World_Step(physicsWorld_, dt, 8);

        // Sync physics positions back to geometry
        auto all_physics = getRegistry()->view<ComponentPhysical, ComponentGeometry>();
        for (auto entity : all_physics) {
            auto [physics, geom] = all_physics.get<ComponentPhysical, ComponentGeometry>(entity);
            b2Vec2 pos = b2Body_GetPosition(physics.body_id);
            geom.x = static_cast<int>(pos.x * ppm - geom.size_x / 2.0f);
            geom.y = static_cast<int>(pos.y * ppm - geom.size_y / 2.0f);
        }
    }
};
}  // namespace will_engine