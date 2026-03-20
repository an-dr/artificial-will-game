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
#include "../containers/Geometry.hpp"

namespace will_engine {

using MapSize = glm::i32vec2;
using TileSize = glm::i32vec2;

template <typename TileType>
class TileMap {
    Atlas2D atlas_;
    std::vector<TileType> tile_descriptor;
    std::string texture_name;

public:
    TileMap(MapSize map_size, TileSize tile_size, TileType default_tile)
        : atlas_(map_size, tile_size) {
        tile_descriptor.resize(map_size.x * map_size.y, default_tile);
    }

    auto load(std::vector<TileType> &&new_descriptor) {
        if (new_descriptor.size() != tile_descriptor.size()) {
            throw std::runtime_error("Wrong size of the new tile descriptor");
        }
        tile_descriptor = std::move(new_descriptor);
    }

    auto setTextureName(std::string name) -> void { texture_name = std::move(name); }
    auto getTextureName() const -> const std::string & { return texture_name; }
    auto getSize() const -> glm::i32vec2 { return atlas_.getAtlasSizePixels(); }
    auto getTileSize() const -> glm::i32vec2 { return atlas_.getTileSize(); }
    auto getTile(int x, int y) const -> TileType {
        auto full_rows = y * atlas_.getAtlasSizePixels().x;
        return tile_descriptor[full_rows + x];
    }
};

}  // namespace will_engine