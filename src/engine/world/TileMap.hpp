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
#include "../containers/TextureAtlas.hpp"

namespace will_engine {

class TileMap {

    // Rendering (Output)
    ArraySize2D size_;
    std::vector<int> tile_descriptor_;
    TileSizePx rendered_tile_size_;

    // Texture (Input)
    std::string texture_name_;
    TextureAtlas tex_atlas;


public:
    TileMap(ArraySize2D map_size_tiles, TileSizePx rendered_tile_px, int default_tile,
            const TextureAtlas &&tex_atlas)
        : size_(map_size_tiles), rendered_tile_size_(rendered_tile_px), tex_atlas(tex_atlas) {
        tile_descriptor_.resize(map_size_tiles.x * map_size_tiles.y, default_tile);
    }

    auto load(std::vector<int> &&new_descriptor) {
        if (new_descriptor.size() != tile_descriptor_.size()) {
            throw std::runtime_error("Wrong size of the new tile descriptor");
        }
        tile_descriptor_ = std::move(new_descriptor);
    }

    [[nodiscard]] auto getTextureName() const -> const std::string & { return tex_atlas.getId(); }
    [[nodiscard]] auto getMapSize() const -> ArraySize2D { return size_; };
    [[nodiscard]] auto getMapTileCount() const -> int { return size_.x * size_.y; }
    [[nodiscard]] auto getTextureTileSize() const -> TileSizePx {
        return tex_atlas.getAtlasSizeTiles();
    }
    [[nodiscard]] auto getRenderTileSize() const -> TileSizePx { return rendered_tile_size_; }
    [[nodiscard]] auto getAtlas() const -> const TextureAtlas & { return tex_atlas; }
    auto getTileType(int x, int y) const -> int { return tile_descriptor_[y * size_.x + x]; }

    auto getTileType(int i) const -> int { return tile_descriptor_[i]; }

    auto getTilePos(int i) const -> glm::ivec2 { return {i % size_.x, i / size_.x}; }

    auto getScreenRect(int i) const -> Rect {
        const auto pos = getTilePos(i);
        return Rect{.position = pos * rendered_tile_size_, .size = rendered_tile_size_};
    }
};

}  // namespace will_engine
