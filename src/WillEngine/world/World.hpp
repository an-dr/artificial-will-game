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

#include "TileMap.hpp"
#include "entity_components/ComponentCollider.hpp"
#include "entity_components/ComponentGeometry.hpp"
#include "entity_components/ComponentInput.hpp"
#include "entity_components/ComponentPlayer.hpp"
#include "entity_components/ComponentSprite.hpp"

namespace will_engine {

struct PlayerCreationResult {
    entt::entity entity;
    unsigned int player_id;
};

template <typename TileType>
class World {
    entt::registry registry_;
    TileMap<TileType> tile_map_;

    auto addEntity(const ComponentGeometry &object) -> entt::entity {
        const auto entity = registry_.create();
        registry_.emplace<ComponentGeometry>(entity, object);
        return entity;
    }

    auto getPlayerId() -> unsigned int {
        auto players = registry_.view<ComponentPlayer>();
        if (players.empty()) {
            return 0;
        }
        return 1;  // TODO
    }

    template <typename Component, typename... Args>
    auto addComponent(entt::entity entity, Args &&...args) -> void {
        registry_.emplace<Component>(entity, std::forward<Args>(args)...);
    }

public:
    World(MapSize map_size, TileSize tile_size, TileType default_tile)
        : tile_map_(map_size, tile_size, default_tile) {};

    auto getRegistry() -> entt::registry * { return &registry_; }
    auto getTileMap() -> TileMap<TileType> * { return &tile_map_; }

    auto add(const ComponentGeometry &geometry, const ComponentSprite &sprite,
             std::optional<const ComponentCollider> collider = std::nullopt) -> entt::entity {
        const auto entity = registry_.create();
        registry_.emplace<ComponentGeometry>(entity, geometry);
        registry_.emplace<ComponentSprite>(entity, sprite);

        if (collider.has_value()) {
            registry_.emplace<ComponentCollider>(entity, collider.value());
        }
        return entity;
    }

    auto addPlayer(const std::string &name, const ComponentGeometry &geometry,
                   const ComponentSprite &sprite, const ComponentCollider &collider)
        -> PlayerCreationResult {
        auto entity = add(geometry, sprite, collider);
        auto id = getPlayerId();
        registry_.emplace<ComponentPlayer>(entity, ComponentPlayer{.name = name, .player_id = id});
        registry_.emplace<ComponentInput>(entity, ComponentInput{});
        return {.entity = entity, .player_id = id};
    }
};

}  // namespace will_engine