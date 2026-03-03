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
public:
    Input() = default;

    auto process(float dt) const -> void {
        const Uint8 *keyState = SDL_GetKeyboardState(nullptr);

        glm::vec2 dir{0.0f, 0.0f};
        if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])  dir.x -= 1.0f;
        if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) dir.x += 1.0f;
        if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])    dir.y -= 1.0f;
        if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN])  dir.y += 1.0f;
        if (glm::length(dir) > 0.0f) dir = glm::normalize(dir);

        constexpr float speed = 160.0f;  // pixels/second
        float dx = dir.x * speed * dt;
        float dy = dir.y * speed * dt;

        auto players = getRegistry()->view<ComponentPlayer, ComponentGeometry, ComponentCollider>();
        auto colliders = getRegistry()->view<ComponentGeometry, ComponentCollider>();

        for (auto player_ent : players) {
            auto &pg = players.get<ComponentGeometry>(player_ent);
            auto &pc = players.get<ComponentCollider>(player_ent);

            pg.x += dx;
            pg.y += dy;

            // Resolve collisions against every other collider
            for (auto other_ent : colliders) {
                if (other_ent == player_ent) continue;

                auto &og = colliders.get<ComponentGeometry>(other_ent);
                auto &oc = colliders.get<ComponentCollider>(other_ent);

                c2AABB pa = makeAABB(pg, pc);
                c2AABB oa = makeAABB(og, oc);

                c2Manifold m = {};
                c2AABBtoAABBManifold(pa, oa, &m);
                if (m.count == 0) continue;

                float depth = m.depths[0];

                if (oc.pushable) {
                    // Try to push the box
                    float new_x = og.x + m.n.x * depth;
                    float new_y = og.y + m.n.y * depth;

                    // Check if pushed position collides with any static collider
                    ComponentGeometry pushed_g = og;
                    pushed_g.x = new_x;
                    pushed_g.y = new_y;
                    c2AABB pushed_aabb = makeAABB(pushed_g, oc);

                    bool blocked = false;
                    for (auto wall_ent : colliders) {
                        if (wall_ent == other_ent || wall_ent == player_ent) continue;
                        auto &wg = colliders.get<ComponentGeometry>(wall_ent);
                        auto &wc = colliders.get<ComponentCollider>(wall_ent);
                        if (wc.pushable) continue;
                        if (c2AABBtoAABB(pushed_aabb, makeAABB(wg, wc))) {
                            blocked = true;
                            break;
                        }
                    }

                    if (!blocked) {
                        og.x = new_x;
                        og.y = new_y;
                    } else {
                        // Box against a wall — treat as static
                        pg.x -= m.n.x * depth;
                        pg.y -= m.n.y * depth;
                    }
                } else {
                    // Static wall — push player out
                    pg.x -= m.n.x * depth;
                    pg.y -= m.n.y * depth;
                }
            }
        }
    }

private:
    static auto makeAABB(const ComponentGeometry &g, const ComponentCollider &c) -> c2AABB {
        float ox = (g.size_x - c.hitbox_w) / 2.0f;
        float oy = (g.size_y - c.hitbox_h) / 2.0f;
        c2AABB aabb;
        aabb.min = {g.x + ox, g.y + oy};
        aabb.max = {g.x + ox + c.hitbox_w, g.y + oy + c.hitbox_h};
        return aabb;
    }
};

}  // namespace will_engine
