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
    static constexpr float PPM = 32.0f;  // pixels per meter

public:
    World() {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = {.x = 0.0f, .y = 0.0f};  // no gravity for top-down
        physicsWorld_ = b2CreateWorld(&worldDef);
    };

    auto getRegistry() -> entt::registry * { return &registry_; }

    auto addNonPhysicalEntity(const ComponentGeometry &object) -> entt::entity {
        const auto entity = registry_.create();
        registry_.emplace<ComponentGeometry>(entity, object);
        return entity;
    }

    auto addPhysicalEntity(const ComponentGeometry &object, b2BodyType body_type = b2_staticBody)
        -> entt::entity {
        const auto entity = registry_.create();

        // Body
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = body_type;
        bodyDef.position = {.x = object.x / PPM, .y = object.y / PPM};
        bodyDef.fixedRotation = true;
        bodyDef.userData = reinterpret_cast<void *>(static_cast<uintptr_t>(entity));
        b2BodyId bodyId = b2CreateBody(physicsWorld_, &bodyDef);

        // Shape
        b2Polygon box = b2MakeBox(object.size_x / PPM / 2.0f, object.size_y / PPM / 2.0f);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        if (body_type != b2_staticBody) {
            shapeDef.density = 1.0f;
            shapeDef.material.friction = 0.0f;
            shapeDef.material.restitution = 0.0f;  // bounciness
        }
        b2CreatePolygonShape(bodyId, &shapeDef, &box);

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