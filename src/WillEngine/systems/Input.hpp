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

#include "../entity_components/ComponentCollider.hpp"
#include "../entity_components/ComponentGeometry.hpp"
#include "../entity_components/ComponentPlayer.hpp"
#include "BaseSystem.hpp"

namespace will_engine {

class Input : public BaseSystem {

    static auto makeAABB(const ComponentGeometry &geometry,
                         const ComponentCollider &collider) -> c2AABB {
        float offset_x = (geometry.size_x - collider.hitbox_w) / 2.0f;
        float offset_y = (geometry.size_y - collider.hitbox_h) / 2.0f;
        c2AABB bounding_box;
        bounding_box.min = {geometry.x + offset_x, geometry.y + offset_y};
        bounding_box.max = {geometry.x + offset_x + collider.hitbox_w,
                            geometry.y + offset_y + collider.hitbox_h};
        return bounding_box;
    }

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

    auto move_player([[maybe_unused]] unsigned int player_id, float delta_time,
                     glm::vec2 direction) {
        auto registry = getRegistry();
        if (!registry)
            return;

        auto players = registry->view<ComponentPlayer>();

        for (auto player_entity : players) {
            auto &player = players.get<ComponentPlayer>(player_entity);
            if (player.player_id == player_id) {
                auto geometry = registry->try_get<ComponentGeometry>(player_entity);
                if (!geometry)
                    throw std::runtime_error("Cannot mova a player. No such entity found");

                constexpr float speed = 160.0f; // pixels/second
                float delta_x = direction.x * speed * delta_time;
                float delta_y = direction.y * speed * delta_time;

                geometry->x += delta_x;
                geometry->y += delta_y;
                return;
            }
        }
    }

    auto resolve_collisions() {
        auto registry = getRegistry();
        auto players = registry->view<ComponentPlayer, ComponentGeometry, ComponentCollider>();
        auto colliders = registry->view<ComponentGeometry, ComponentCollider>();

        for (auto player_entity : players) {
            auto &player_geometry = players.get<ComponentGeometry>(player_entity);
            auto &player_collider = players.get<ComponentCollider>(player_entity);

            for (auto collider_entity : colliders) {
                if (collider_entity == player_entity)
                    continue;

                auto &other_geometry = colliders.get<ComponentGeometry>(collider_entity);
                auto &other_collider = colliders.get<ComponentCollider>(collider_entity);

                c2AABB player_aabb = makeAABB(player_geometry, player_collider);
                c2AABB other_aabb = makeAABB(other_geometry, other_collider);

                c2Manifold manifold = {};
                c2AABBtoAABBManifold(player_aabb, other_aabb, &manifold);
                if (manifold.count == 0)
                    continue;

                float penetration_depth = manifold.depths[0];

                if (other_collider.pushable) {
                    // Try to push the box
                    float pushed_x = other_geometry.x + manifold.n.x * penetration_depth;
                    float pushed_y = other_geometry.y + manifold.n.y * penetration_depth;

                    // Check if pushed position collides with any static collider
                    ComponentGeometry pushed_geometry = other_geometry;
                    pushed_geometry.x = pushed_x;
                    pushed_geometry.y = pushed_y;
                    c2AABB pushed_aabb = makeAABB(pushed_geometry, other_collider);

                    bool blocked = false;
                    for (auto wall_entity : colliders) {
                        if (wall_entity == collider_entity || wall_entity == player_entity)
                            continue;
                        auto &wall_geometry = colliders.get<ComponentGeometry>(wall_entity);
                        auto &wall_collider = colliders.get<ComponentCollider>(wall_entity);
                        if (wall_collider.pushable)
                            continue;
                        if (c2AABBtoAABB(pushed_aabb, makeAABB(wall_geometry, wall_collider))) {
                            blocked = true;
                            break;
                        }
                    }

                    if (!blocked) {
                        other_geometry.x = pushed_x;
                        other_geometry.y = pushed_y;
                    } else {
                        // Box against a wall — treat as static
                        player_geometry.x -= manifold.n.x * penetration_depth;
                        player_geometry.y -= manifold.n.y * penetration_depth;
                    }
                } else {
                    // Static wall — push player out
                    player_geometry.x -= manifold.n.x * penetration_depth;
                    player_geometry.y -= manifold.n.y * penetration_depth;
                }
            }
        }
    }

public:
    Input() = default;

    auto process(float delta_time) -> void {
        auto players = getRegistry()->view<ComponentPlayer, ComponentGeometry, ComponentCollider>();

        for (auto player_entity : players) {
            auto &player = players.get<ComponentPlayer>(player_entity);

            auto direction = get_direction(player.player_id);
            move_player(player.player_id, delta_time, direction);

            resolve_collisions();
        }
    }

};

} // namespace will_engine