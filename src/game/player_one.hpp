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

#include "../engine/Game.hpp"
#include "../engine/systems/BaseStateMashine.hpp"
#include "../engine/world/World.hpp"
#include "../engine/world/entity_components/ComponentCollider.hpp"
#include "../engine/world/entity_components/ComponentGeometry.hpp"
#include "../engine/world/entity_components/ComponentInput.hpp"
#include "../engine/world/entity_components/ComponentSpriteRendering.hpp"

using namespace will_engine;

namespace player_sm {

// Transition table
struct Table {
    // States
    struct Idle {};
    struct Moving {};

    // Events
    struct EvStartMove {};
    struct EvStop {};

    // Actions
    struct on_start_move {
        SpriteSharedPtr moving_sprite;

        auto operator()(IStateMachine &ctx) const -> void {
            auto &sprite = ctx.getRegistry()->get<ComponentSpriteRendering>(ctx.getEntittyId());
            sprite.setSprite(moving_sprite);
        }
    };

    struct on_stop {
        SpriteSharedPtr idle_sprite;

        auto operator()(IStateMachine &ctx) const -> void {
            auto &sprite = ctx.getRegistry()->get<ComponentSpriteRendering>(ctx.getEntittyId());
            sprite.setSprite(idle_sprite);
        }
    };

    SpriteSharedPtr idle_sprite;
    SpriteSharedPtr moving_sprite;

    auto operator()() const {
        using namespace boost::sml;
        return make_transition_table(  //
            *state<Idle> + event<EvStartMove> / on_start_move{moving_sprite} = state<Moving>,
            state<Moving> + event<EvStop> / on_stop{idle_sprite} = state<Idle>  //
        );
    }
};
}  // namespace player_sm

class StateMachinePlayer : public BaseStateMashine<player_sm::Table> {

public:
    StateMachinePlayer(entt::entity id, SpriteSharedPtr idle_sprite, SpriteSharedPtr moving_sprite)
        : BaseStateMashine(id, player_sm::Table{
              .idle_sprite = std::move(idle_sprite), .moving_sprite = std::move(moving_sprite)}) {}
    auto tick() -> void override {
        if (getRegistry() == nullptr)
            return;
        auto &[input] = getRegistry()->get<ComponentInput>(getEntittyId());
        if (input.x != 0 || input.y != 0)
            process(player_sm::Table::EvStartMove{});  // TODO: wrap process into something to
                                                       // update the registry state
        else
            process(player_sm::Table::EvStop{});
    }
};


auto build_player_one(Game &game, World &world, float x, float y) {
    auto player_tex =
        game.loadTexture("player::walk::down", "assets/RPGMCharacter_v1.0/_down walk.png");
    auto player_idle =
        game.loadTexture("player::idle::down", "assets/RPGMCharacter_v1.0/_down idle.png");

    auto sprite_idle = std::make_shared<Sprite>(
        Atlas2D{player_idle, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});
    auto sprite_walk = std::make_shared<Sprite>(
        Atlas2D{player_tex, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});


    auto [entity, id] = world.addPlayer(
        "Player One",
        ComponentGeometry{.x = x, .y = y, .z = 0, .size_x = 128, .size_y = 128, .size_z = 0},
        ComponentSpriteRendering{.sprite = sprite_idle, .frame_float = 0.0f},
        ComponentCollider{});

    auto sm = std::make_unique<StateMachinePlayer>(entity, sprite_idle, sprite_walk);
    game.addStateMachine(std::move(sm));
    return entity;
}
