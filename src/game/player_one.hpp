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

    auto operator()() const {
        using namespace boost::sml;
        return make_transition_table(  //
            *state<Idle> + event<EvStartMove> = state<Moving>,
            state<Moving> + event<EvStop> = state<Idle>  //
        );
    }
};
}  // namespace player_sm

enum class PlayerFacing { Down, Up, Left, Right };

struct PlayerSpriteSet {
    SpriteSharedPtr idle_down;
    SpriteSharedPtr idle_up;
    SpriteSharedPtr idle_side;
    SpriteSharedPtr walk_down;
    SpriteSharedPtr walk_up;
    SpriteSharedPtr walk_side;
};

class StateMachinePlayer : public BaseStateMashine<player_sm::Table> {
    PlayerSpriteSet sprites_;
    PlayerFacing facing_ = PlayerFacing::Down;

    auto updateFacing(const glm::vec2 &input) -> void {
        if (input.x == 0.0f && input.y == 0.0f)
            return;

        if (std::abs(input.x) >= std::abs(input.y)) {
            facing_ = input.x < 0.0f ? PlayerFacing::Left : PlayerFacing::Right;
            return;
        }

        facing_ = input.y < 0.0f ? PlayerFacing::Up : PlayerFacing::Down;
    }

    auto updateSpritePresentation(const glm::vec2 &input) -> void {
        auto *registry = getRegistry();
        if (registry == nullptr)
            return;

        updateFacing(input);

        const bool moving = input.x != 0.0f || input.y != 0.0f;
        auto &rendering = registry->get<ComponentSpriteRendering>(getEntittyId());

        SpriteSharedPtr next_sprite;
        SpriteTransform next_transform{};

        switch (facing_) {
            case PlayerFacing::Down:
                next_sprite = moving ? sprites_.walk_down : sprites_.idle_down;
                break;
            case PlayerFacing::Up:
                next_sprite = moving ? sprites_.walk_up : sprites_.idle_up;
                break;
            case PlayerFacing::Left:
                next_sprite = moving ? sprites_.walk_side : sprites_.idle_side;
                break;
            case PlayerFacing::Right:
                next_sprite = moving ? sprites_.walk_side : sprites_.idle_side;
                next_transform.flip_horizontal = true;
                break;
        }

        rendering.setSprite(std::move(next_sprite));
        rendering.setTransform(next_transform);
    }

public:
    StateMachinePlayer(entt::entity id, PlayerSpriteSet sprites)
        : BaseStateMashine(id), sprites_(std::move(sprites)) {}
    auto tick() -> void override {
        if (getRegistry() == nullptr)
            return;
        const auto &input = getRegistry()->get<ComponentInput>(getEntittyId()).input_keyboard;
        updateSpritePresentation(input);
        if (input.x != 0 || input.y != 0)
            process(player_sm::Table::EvStartMove{});  // TODO: wrap process into something to
                                                       // update the registry state
        else
            process(player_sm::Table::EvStop{});
    }
};


auto build_player_one(Game &game, World &world, float x, float y) {
    auto player_walk_down =
        game.loadTexture("player::walk::down", "assets/RPGMCharacter_v1.0/_down walk.png");
    auto player_idle_down =
        game.loadTexture("player::idle::down", "assets/RPGMCharacter_v1.0/_down idle.png");
    auto player_walk_side =
        game.loadTexture("player::walk::side", "assets/RPGMCharacter_v1.0/_side walk.png");
    auto player_idle_side =
        game.loadTexture("player::idle::side", "assets/RPGMCharacter_v1.0/_side idle.png");
    auto player_walk_up = game.loadTexture("player::walk::up", "assets/RPGMCharacter_v1.0/_up walk.png");
    auto player_idle_up = game.loadTexture("player::idle::up", "assets/RPGMCharacter_v1.0/_up idle.png");

    auto sprite_idle_down = std::make_shared<Sprite>(
        Atlas2D{player_idle_down, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});
    auto sprite_walk_down = std::make_shared<Sprite>(
        Atlas2D{player_walk_down, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});
    auto sprite_idle_side = std::make_shared<Sprite>(
        Atlas2D{player_idle_side, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});
    auto sprite_walk_side = std::make_shared<Sprite>(
        Atlas2D{player_walk_side, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});
    auto sprite_idle_up = std::make_shared<Sprite>(
        Atlas2D{player_idle_up, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});
    auto sprite_walk_up = std::make_shared<Sprite>(
        Atlas2D{player_walk_up, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});


    auto [entity, id] = world.addPlayer(
        "Player One",
        ComponentGeometry{.x = x, .y = y, .z = 0, .size_x = 128, .size_y = 128, .size_z = 0},
        ComponentSpriteRendering{.sprite = sprite_idle_down, .frame_float = 0.0f},
        ComponentCollider{});

    auto sm = std::make_unique<StateMachinePlayer>(
        entity, PlayerSpriteSet{.idle_down = sprite_idle_down,
                                .idle_up = sprite_idle_up,
                                .idle_side = sprite_idle_side,
                                .walk_down = sprite_walk_down,
                                .walk_up = sprite_walk_up,
                                .walk_side = sprite_walk_side});
    game.addStateMachine(std::move(sm));
    return entity;
}
