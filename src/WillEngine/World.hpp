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

#include "entity_components/ComponentObject.hpp"

namespace will_engine
{
    class World
    {
        entt::registry registry_; // All entities/components here

    public:
        World()
        {
            auto entity = registry_.create();
            registry_.emplace<int>(entity, 1);
        }


        auto addEntity(const ComponentObject& object) -> entt::entity
        {
            const auto entity = registry_.create();
            registry_.emplace<ComponentObject>(entity, object);
            return entity;
        }

        template <typename Component>
        auto add(entt::entity entity_id, const Component& component) -> void
        {
            registry_.emplace<Component>(entity_id, component);
        }
    };
}
