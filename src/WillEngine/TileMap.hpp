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
#include <stdexcept>
#include <string>
#include <vector>
#include <glm/vec2.hpp>

namespace will_engine {

using MapSize = glm::i32vec2;
using TileSize = glm::i32vec2;

template <typename TileType>
class TileMap {
    glm::i32vec2 size{0, 0};
    glm::i32vec2 tileSize{0, 0};          // tile size in the texture (src)
    glm::i32vec2 displayTileSize{0, 0};   // tile size drawn on screen (dst)
    std::vector<TileType> tile_descriptor;
    std::string texture_name;

public:
    TileMap(MapSize map_size, TileSize tile_size, TileType default_tile)
        : size(map_size), tileSize(tile_size), displayTileSize(tile_size) {
        tile_descriptor.resize(map_size.x * map_size.y, default_tile);
    }

    auto load(std::vector<TileType> &&new_descriptor) {
        if (new_descriptor.size() != tile_descriptor.size()) {
            throw std::runtime_error("Wring size of the new tile descriptor");
        }
        tile_descriptor = std::move(new_descriptor);
    }

    auto setTextureName(std::string name) -> void { texture_name = std::move(name); }
    auto getTextureName() const -> const std::string & { return texture_name; }
    auto getSize() const -> glm::i32vec2 { return size; }
    auto getTileSize() const -> glm::i32vec2 { return tileSize; }
    auto setDisplayTileSize(glm::i32vec2 s) -> void { displayTileSize = s; }
    auto getDisplayTileSize() const -> glm::i32vec2 { return displayTileSize; }
    auto getTile(int x, int y) const -> TileType { return tile_descriptor[y * size.x + x]; }
};

}  // namespace will_engine