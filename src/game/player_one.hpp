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

#include <cmath>

#include "../engine/Game.hpp"
#include "../engine/systems/BaseStateMachine.hpp"
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
    struct Attacking {};

    // Events
    struct EvStartMove {};
    struct EvStop {};
    struct EvStartAttack {};
    struct EvStopAttack {};

    auto operator()() const {
        using namespace boost::sml;
        return make_transition_table(  //
            *state<Idle> + event<EvStartMove> = state<Moving>,
            state<Idle> + event<EvStartAttack> = state<Attacking>,
            state<Moving> + event<EvStop> = state<Idle>,
            state<Moving> + event<EvStartAttack> = state<Attacking>,
            state<Attacking> + event<EvStopAttack> = state<Idle>  //
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
    SpriteSharedPtr attack_side;
    SpriteSharedPtr attack_up;
    SpriteSharedPtr attack_down;
};

class StateMachinePlayer : public BaseStateMachine<player_sm::Table> {
    PlayerSpriteSet sprites_;
    PlayerFacing facing_ = PlayerFacing::Down;
    bool attack_button_was_pressed_ = false;

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

        auto &rendering = registry->get<ComponentSpriteRendering>(getEntityId());
        const bool attacking = isState<player_sm::Table::Attacking>();
        const bool moving = isState<player_sm::Table::Moving>();

        if (!attacking) {
            updateFacing(input);
        }

        SpriteSharedPtr next_sprite;
        SpriteTransform next_transform{};

        switch (facing_) {
        case PlayerFacing::Down:
            if (attacking) {
                next_sprite = sprites_.attack_down;
            } else {
                next_sprite = moving ? sprites_.walk_down : sprites_.idle_down;
            }
            break;
        case PlayerFacing::Up:
            if (attacking) {
                next_sprite = sprites_.attack_up;
            } else {
                next_sprite = moving ? sprites_.walk_up : sprites_.idle_up;
            }
            break;
        case PlayerFacing::Left:
            if (attacking) {
                next_sprite = sprites_.attack_side;
            } else {
                next_sprite = moving ? sprites_.walk_side : sprites_.idle_side;
            }
            break;
        case PlayerFacing::Right:
            if (attacking) {
                next_sprite = sprites_.attack_side;
            } else {
                next_sprite = moving ? sprites_.walk_side : sprites_.idle_side;
            }
            next_transform.flip_horizontal = true;
            break;
        }

        rendering.setSprite(std::move(next_sprite));
        rendering.setAnimationLoop(!attacking);
        rendering.setTransform(next_transform);
    }

    auto syncLocomotionState(const ComponentInput &input) -> void {
        if (input.input_keyboard.x != 0 || input.input_keyboard.y != 0)
            process(player_sm::Table::EvStartMove{});
        else
            process(player_sm::Table::EvStop{});
    }

public:
    StateMachinePlayer(entt::entity id, PlayerSpriteSet sprites)
        : BaseStateMachine(id), sprites_(std::move(sprites)) {}
    auto tick() -> void override {
        if (getRegistry() == nullptr)
            return;

        const auto &input = getRegistry()->get<ComponentInput>(getEntityId());
        auto &rendering = getRegistry()->get<ComponentSpriteRendering>(getEntityId());
        const bool attack_just_pressed = input.attack_pressed && !attack_button_was_pressed_;
        attack_button_was_pressed_ = input.attack_pressed;

        if (isState<player_sm::Table::Attacking>()) {
            if (rendering.isAnimationFinished()) {
                process(player_sm::Table::EvStopAttack{});
                syncLocomotionState(input);
            }
        } else if (attack_just_pressed) {
            process(player_sm::Table::EvStartAttack{});
        } else {
            syncLocomotionState(input);
        }

        updateSpritePresentation(input.input_keyboard);
    }
};


auto build_player_one(Game &game, World &world, float x, float y) {

    // player::idle::up
    //  ----------------
    auto texture_idle_up = game.loadTexture("player::idle::up",  //
                                            "assets/RPGMCharacter_v1.0/_up idle.png");
    auto sprite_idle_up = std::make_shared<Sprite>(
        Atlas2D{texture_idle_up, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});

    // player::idle::down
    // ----------------
    auto texture_idle_down = game.loadTexture("player::idle::down",  //
                                              "assets/RPGMCharacter_v1.0/_down idle.png");
    auto sprite_idle_down = std::make_shared<Sprite>(
        Atlas2D{texture_idle_down, AtlasSizePx{256, 128}, TileSizePx{64, 64}},  //
        SpiteHitbox{20, 54},                                                    //
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});

    // player::idle::side
    //  ----------------
    auto texture_idle_side = game.loadTexture("player::idle::side",  //
                                              "assets/RPGMCharacter_v1.0/_side idle.png");
    auto sprite_idle_side = std::make_shared<Sprite>(
        Atlas2D{texture_idle_side, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});

    // player::walk::side
    // ----------------
    auto texture_walk_side = game.loadTexture("player::walk::side",  //
                                              "assets/RPGMCharacter_v1.0/_side walk.png");
    auto sprite_walk_side = std::make_shared<Sprite>(
        Atlas2D{texture_walk_side, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});

    // player::walk::up
    // ----------------
    auto texture_walk_up = game.loadTexture("player::walk::up",  //
                                            "assets/RPGMCharacter_v1.0/_up walk.png");
    auto sprite_walk_up = std::make_shared<Sprite>(
        Atlas2D{texture_walk_up, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});

    // player::walk::down
    // ----------------
    auto texture_walk_down = game.loadTexture("player::walk::down",  //
                                              "assets/RPGMCharacter_v1.0/_down walk.png");
    auto sprite_walk_down = std::make_shared<Sprite>(
        Atlas2D{texture_walk_down, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 5u});


    // player::attack::side
    // ----------------
    auto texture_attack_side = game.loadTexture("player::attack::side",  //
                                                "assets/RPGMCharacter_v1.0/_side attack.png");
    auto sprite_attack_side = std::make_shared<Sprite>(
        Atlas2D{texture_attack_side, AtlasSizePx{256, 128}, TileSizePx{64, 64}},
        SpiteHitbox{20, 54}, SpriteAnimationDescriptor{.fps = 8u, .frame_count = 2u});

    // player::attack::up
    // ----------------
    auto texture_attack_up = game.loadTexture("player::attack::up",  //
                                              "assets/RPGMCharacter_v1.0/_up attack.png");
    auto sprite_attack_up = std::make_shared<Sprite>(
        Atlas2D{texture_attack_up, AtlasSizePx{256, 128}, TileSizePx{64, 64}}, SpiteHitbox{20, 54},
        SpriteAnimationDescriptor{.fps = 8u, .frame_count = 2u});

    // player::attack::down
    // ----------------
    auto texture_attack_down = game.loadTexture("player::attack::down",  //
                                                "assets/RPGMCharacter_v1.0/_down attack.png");
    auto sprite_attack_down = std::make_shared<Sprite>(
        Atlas2D{texture_attack_down, AtlasSizePx{256, 128}, TileSizePx{64, 64}},
        SpiteHitbox{20, 54}, SpriteAnimationDescriptor{.fps = 8u, .frame_count = 2u});

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
                                .walk_side = sprite_walk_side,
                                .attack_side = sprite_attack_side,
                                .attack_up = sprite_attack_up,
                                .attack_down = sprite_attack_down});
    game.addStateMachine(std::move(sm));
    return entity;
}
