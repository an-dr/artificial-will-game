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
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


namespace will_engine::geometry {

struct Rect {
    glm::ivec2 a, b;
};

struct Box {
    glm::ivec3 a, b;
};

class Atlas2D {

    int size_x_px_;
    int size_y_px_;
    int tile_size_x_;
    int tile_size_y_;
    int size_x_tiles_;
    int size_y_tiles_;

public:
    Atlas2D(const int atlas_x_px, const int atlas_y_px, const int tile_x_px, const int tile_y_px)
        : size_x_px_(atlas_x_px),                 //
          size_y_px_(atlas_y_px),                 //
          tile_size_x_(tile_x_px),                //
          tile_size_y_(tile_y_px),                //
          size_x_tiles_(atlas_x_px / tile_x_px),  //
          size_y_tiles_(atlas_y_px / tile_y_px) {}

    [[nodiscard]] auto getTile(const int col, const int row) const -> Rect {
        if (col >= size_x_tiles_ || row >= size_y_tiles_) {
            return Rect{};
        }

        int a_x = col * tile_size_x_;
        int a_y = row * tile_size_y_;

        return {.a = {a_x, a_y}, .b = {a_x + tile_size_x_, a_y + tile_size_y_}};
    }

    [[nodiscard]] auto getTile(const int i) const -> Rect {
        if (i >= size_x_tiles_ * size_y_tiles_) {
            return Rect{};
        }

        int col = i % size_x_tiles_;
        int row = i / size_x_tiles_;

        return getTile(col, row);
    }

    [[nodiscard]] auto getAtlasSize() const { return glm::ivec2{size_x_px_, size_y_px_}; }
    [[nodiscard]] auto getTileSize() const { return glm::ivec2{tile_size_x_, tile_size_y_}; }
    [[nodiscard]] auto getSizeInTilesY() const { return size_y_tiles_; }
    [[nodiscard]] auto getSizeInTilesX() const { return size_x_tiles_; }
};

}  // namespace will_engine::geometry
