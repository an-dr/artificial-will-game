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
#include <optional>
#include <glm/glm.hpp>

namespace will_engine {

struct Rect {
    glm::ivec2 position, size;
};

struct Box {
    glm::ivec3 a, b;
    [[nodiscard]] auto getSizes() const { return glm::abs(glm::ivec3{b.x - a.x, b.y - a.y, b.z - a.z}); }
};

using ArraySize2D = glm::ivec2;
using AtlasSizePx = glm::ivec2;
using TileSizePx = glm::ivec2;


class Atlas2D {

    glm::ivec2 atlas_size_;
    glm::ivec2 tile_size_;
    glm::ivec2 tiles_num_;

    int size_total_tiles_=0;

public:
    Atlas2D(const AtlasSizePx atlas_size, const TileSizePx tile_size)
        : atlas_size_(atlas_size), tile_size_(tile_size),
          tiles_num_{(atlas_size.x / tile_size.x), (atlas_size.y / tile_size.y)},
          size_total_tiles_((atlas_size.x / tile_size.x) * (atlas_size.y / tile_size.y)) {}

    [[nodiscard]] auto getTile(const int col, const int row) const -> std::optional<Rect> {
        if (col >= tiles_num_.x || row >= tiles_num_.y) {
            return std::nullopt;
        }

        int a_x = col * tile_size_.x;
        int a_y = row * tile_size_.y;

        return Rect{.position = {a_x, a_y}, .size = {tile_size_.x, tile_size_.y}};
    }

    [[nodiscard]] auto getTile(const int i) const -> std::optional<Rect> {
        if (i >= tiles_num_.x * tiles_num_.y) {
            return std::nullopt;
        }

        int col = i % tiles_num_.x;
        int row = i / tiles_num_.x;

        return getTile(col, row);
    }

    [[nodiscard]] auto getAtlasSizePixels() const { return atlas_size_; }
    [[nodiscard]] auto getAtlasSizeTiles() const { return glm::ivec2{tiles_num_.x, tiles_num_.y}; }
    [[nodiscard]] auto getTileSize() const { return glm::ivec2{tile_size_.x, tile_size_.y}; }
};

}  // namespace will_engine
