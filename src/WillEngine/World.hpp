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
#include <box2d/box2d.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "entity_components/ComponentGeometry.hpp"
#include "entity_components/ComponentPhysical.hpp"

namespace will_engine {

class World {
    entt::registry registry_;  // All entities/components here
    b2WorldId physicsWorld_;

public:
    static constexpr float PPM = 32.0f;  // pixels per meter

    World() {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = {.x = 0.0f, .y = 0.0f};  // no gravity for top-down
        physicsWorld_ = b2CreateWorld(&worldDef);
    };

    auto getRegistry() -> entt::registry * { return &registry_; }

    auto getPhysicsWorld() const { return physicsWorld_; };

    auto addNonPhysicalEntity(const ComponentGeometry &object) -> entt::entity {
        const auto entity = registry_.create();
        registry_.emplace<ComponentGeometry>(entity, object);
        return entity;
    }

    auto addPhysicalEntity(const ComponentGeometry &object, b2BodyType body_type = b2_staticBody,
                           int hitbox_x = 0, int hitbox_y = 0,
                           int hitbox_shift_x = 0, int hitbox_shift_y = 0,
                           float mass = 1.0f, float friction = 0.0f,
                           float linear_damping = 0.0f, float hitbox_radius = 0.0f) -> entt::entity {
        const auto entity = registry_.create();

        int phys_x = hitbox_x > 0 ? hitbox_x : object.size_x;
        int phys_y = hitbox_y > 0 ? hitbox_y : object.size_y;

        // Body
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = body_type;
        bodyDef.position = {.x = (object.x + object.size_x / 2.0f + hitbox_shift_x) / PPM,
                            .y = (object.y + object.size_y / 2.0f + hitbox_shift_y) / PPM};
        bodyDef.fixedRotation = true;
        bodyDef.linearDamping = linear_damping;
        bodyDef.userData = reinterpret_cast<void *>(static_cast<uintptr_t>(entity));
        b2BodyId bodyId = b2CreateBody(physicsWorld_, &bodyDef);

        // Shape — rounded corners eliminate diagonal contact normals at edges/corners
        float hx = phys_x / PPM / 2.0f;
        float hy = phys_y / PPM / 2.0f;
        b2Polygon box = hitbox_radius > 0.0f
                            ? b2MakeRoundedBox(hx - hitbox_radius, hy - hitbox_radius, hitbox_radius)
                            : b2MakeBox(hx, hy);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        if (body_type != b2_staticBody) {
            shapeDef.density = 1.0f;
            shapeDef.material.friction = friction;
            shapeDef.material.restitution = 0.0f;
        }
        b2CreatePolygonShape(bodyId, &shapeDef, &box);

        // Override mass for dynamic bodies (kinematic mass has no effect)
        if (body_type == b2_dynamicBody) {
            b2Body_SetMassData(bodyId, {.mass = mass,
                                        .center = {0.0f, 0.0f},
                                        .rotationalInertia = mass});
        }

        // Add
        registry_.emplace<ComponentPhysical>(entity, bodyId);
        registry_.emplace<ComponentGeometry>(entity, object);
        return entity;
    }

    template <typename Component, typename... Args>
    auto addComponent(entt::entity entity, Args &&...args) -> void {
        registry_.emplace<Component>(entity, std::forward<Args>(args)...);
    }
};

}  // namespace will_engine