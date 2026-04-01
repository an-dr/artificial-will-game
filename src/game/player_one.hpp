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
        auto operator()(IStateMachine &ctx) const -> void {
            ctx.getRegistry()->get<ComponentSpriteRendering>(ctx.getEntittyId()).type = SpriteType::Animated;
        }
    };

    struct on_stop {
        auto operator()(IStateMachine &ctx) const -> void {
            auto &sprite = ctx.getRegistry()->get<ComponentSpriteRendering>(ctx.getEntittyId());
            // sprite.type = SpriteType::Static;
            // sprite.frame_float = 2.0f;
        }
    };

    auto operator()() const {
        using namespace boost::sml;
        return make_transition_table(  //
            *state<Idle> + event<EvStartMove> / on_start_move{} = state<Moving>,
            state<Moving> + event<EvStop> / on_stop{} = state<Idle>  //
        );
    }
};
}  // namespace player_sm

class StateMachinePlayer : public BaseStateMashine<player_sm::Table> {

public:
    explicit StateMachinePlayer(entt::entity id) : BaseStateMashine(id) {}
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
    auto player_tex = game.loadTexture("player::walk::down", "assets/RPGMCharacter_v1.0/_down walk.png");
    auto player_idle = game.loadTexture("player::idle::down", "assets/RPGMCharacter_v1.0/_down idle.png");

    auto [entity, id] = world.addPlayer(
        "Player One",
        ComponentGeometry{.x = x, .y = y, .z = 0, .size_x = 128, .size_y = 128, .size_z = 0},
        ComponentSpriteRendering{.sprite = Sprite{player_idle, AtlasSizePx{256, 128}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Animated,
                        .fps = 8u,
                        .frame_count = 5u},
        ComponentCollider{.hitbox_w = 20, .hitbox_h = 54});

    auto sm = std::make_unique<StateMachinePlayer>(entity);
    game.addStateMachine(std::move(sm));
    return entity;
}
