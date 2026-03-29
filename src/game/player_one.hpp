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
#include "../engine/world/entity_components/ComponentSprite.hpp"

using namespace will_engine;

namespace player_sm {
// States
struct Idle {};
struct Moving {};

// Events
struct EvStartMove {};
struct EvStop {};

// Transition table
struct Table {
    struct OnStartMove {
        auto operator()(IStateMachine &ctx) const -> void {
            ctx.getRegistry()->get<ComponentSprite>(ctx.getEntittyId()).type = SpriteType::Animated;
        }
    };

    struct OnStop {
        auto operator()(IStateMachine &ctx) const -> void {
            auto &sprite = ctx.getRegistry()->get<ComponentSprite>(ctx.getEntittyId());
            sprite.type = SpriteType::Static;
            sprite.frame_float = 0.0f;
        }
    };

    auto operator()() const {
        using namespace boost::sml;
        return make_transition_table(  //
            *state<Idle> + event<EvStartMove> / OnStartMove{} = state<Moving>,
            state<Moving> + event<EvStop> / OnStop{} = state<Idle>  //
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
            process(player_sm::EvStartMove{});  // TODO: wrap process into something to update the
                                                // registry state
        else
            process(player_sm::EvStop{});
    }
};


template <typename TileType>
auto build_player_one(Game &game, World<TileType> &world, float x, float y) {
    auto player_tex = game.loadTexture("player.png", "assets/robot_william.png");

    auto [entity, id] = world.addPlayer(
        "Player One",
        ComponentGeometry{.x = x, .y = y, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas = TextureAtlas{player_tex, AtlasSizePx{256, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Static,
                        .fps = 8u},
        ComponentCollider{.hitbox_w = 40, .hitbox_h = 56});

    auto sm = std::make_unique<StateMachinePlayer>(entity);
    game.addStateMachine(std::move(sm));
    return entity;
}
