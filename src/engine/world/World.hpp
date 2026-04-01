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

#include "CameraState.hpp"
#include "TileMap.hpp"
#include "entity_components/ComponentCameraTarget.hpp"
#include "entity_components/ComponentCollider.hpp"
#include "entity_components/ComponentGeometry.hpp"
#include "entity_components/ComponentInput.hpp"
#include "entity_components/ComponentPlayer.hpp"
#include "entity_components/ComponentSpriteRendering.hpp"
#include "entity_components/ComponentType.hpp"

namespace will_engine {

struct PlayerCreationResult {
    entt::entity entity;
    unsigned int player_id;
};

class World {
    entt::registry registry_;
    std::unique_ptr<TileMap> tile_map_;
    CameraState camera_state_;

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
    World() = default;

    auto getRegistry() -> entt::registry * { return &registry_; }
    auto getCameraState() -> CameraState * { return &camera_state_; }
    auto getTileMap() -> TileMap * { return tile_map_.get(); }
    auto setTileMap(TileMap &&tile_map) {
        tile_map_ = std::make_unique<TileMap>(std::move(tile_map));
    }

    auto add(const ComponentGeometry &geometry, const ComponentSpriteRendering &sprite,
             std::optional<const ComponentCollider> collider = std::nullopt) -> entt::entity {
        const auto entity = registry_.create();
        registry_.emplace<ComponentGeometry>(entity, geometry);
        registry_.emplace<ComponentSpriteRendering>(entity, sprite);
        registry_.emplace<ComponentType>(entity, ComponentType{});

        if (collider.has_value()) {
            registry_.emplace<ComponentCollider>(entity, collider.value());
        }
        return entity;
    }

    auto addPlayer(const std::string &name, const ComponentGeometry &geometry,
                   const ComponentSpriteRendering &sprite, const ComponentCollider &collider)
        -> PlayerCreationResult {
        auto entity = add(geometry, sprite, collider);

        // update type to player
        auto &type = registry_.get<ComponentType>(entity);
        type.type = EntityType::Player;

        auto id = getPlayerId();
        registry_.emplace<ComponentPlayer>(entity, ComponentPlayer{.name = name, .player_id = id});
        registry_.emplace<ComponentInput>(entity, ComponentInput{});
        registry_.emplace<ComponentCameraTarget>(entity, ComponentCameraTarget{.follow = true});
        return {.entity = entity, .player_id = id};
    }

};

}  // namespace will_engine
