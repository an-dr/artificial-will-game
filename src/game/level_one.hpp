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
#include "../engine/world/entity_components/ComponentCollider.hpp"
#include "../engine/world/entity_components/ComponentSprite.hpp"
#include "player_one.hpp"

using namespace will_engine;

template <typename TileType>
void build_tile_map(Game &game, World<TileType> &world) {

    auto tiles_tex = game.loadTexture(
        "tiles.png", "assets/Pixel Art Top Down - Basic v1.2.3/Texture/TX Tileset Grass.png");

    auto tile_map =
        TileMap<int>{ArraySize2D{16, 16}, TileSizePx{64, 64}, 0,
                     TextureAtlas{tiles_tex, AtlasSizePx{256, 256}, TileSizePx{64, 64}}};
    std::vector<int> tile_descriptor = {};
    tile_descriptor.resize(16 * 16);
    tile_descriptor[16 * 2 + 2] = 3;
    tile_descriptor[16 * 2 + 3] = 3;
    tile_descriptor[16 * 2 + 5] = 3;
    tile_descriptor[16 * 8 + 2] = 3;
    tile_descriptor[16 * 4 + 8] = 8;
    tile_descriptor[16 * 4 + 9] = 9;
    tile_map.load(std::move(tile_descriptor));
    world.setTileMap(std::move(tile_map));
}

template <typename TileType>
void build_boxes(Game &game, World<TileType> &world) {

    auto texture_name = game.loadTexture("box.png", "assets/box.png");

    world.add(
        ComponentGeometry{.x = 210, .y = 410, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas =
                            TextureAtlas{texture_name, AtlasSizePx{64, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Static,
                        .fps = 0u},
        ComponentCollider{.hitbox_w = 40, .hitbox_h = 56, .pushable = true});

    world.add(
        ComponentGeometry{.x = 400, .y = 500, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas =
                            TextureAtlas{texture_name, AtlasSizePx{64, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Static,
                        .fps = 0u},
        ComponentCollider{.hitbox_w = 40, .hitbox_h = 56, .pushable = true});

    world.add(
        ComponentGeometry{.x = 500, .y = 400, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas =
                            TextureAtlas{texture_name, AtlasSizePx{64, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Static,
                        .fps = 0u},
        ComponentCollider{.hitbox_w = 40, .hitbox_h = 56, .pushable = true});
}

auto build_level_one(Game &game) -> std::unique_ptr<World<int>> {
    auto world = std::make_unique<World<int>>();
    build_tile_map(game, *world);
    build_boxes(game, *world);
    build_player_one(game, *world, 400, 400);

    return world;
}