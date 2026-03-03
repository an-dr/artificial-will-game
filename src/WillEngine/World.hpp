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
#include <entt/entt.hpp>

#include "entity_components/ComponentGeometry.hpp"

namespace will_engine {

class World {
    entt::registry registry_;

public:
    World() = default;

    auto getRegistry() -> entt::registry * { return &registry_; }

    auto addEntity(const ComponentGeometry &object) -> entt::entity {
        const auto entity = registry_.create();
        registry_.emplace<ComponentGeometry>(entity, object);
        return entity;
    }

    template <typename Component, typename... Args>
    auto addComponent(entt::entity entity, Args &&...args) -> void {
        registry_.emplace<Component>(entity, std::forward<Args>(args)...);
    }
};

}  // namespace will_engine
