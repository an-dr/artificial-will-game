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
#include <string>
#include <utility>
#include "../containers/Geometry.hpp"

enum class SpriteType { Static, Animated };

namespace will_engine {

class Sprite : public Atlas2D {
    std::string texture_id_;
    // TODO: move ComponentSpriteRendering components specific the animation here

public:
    Sprite(std::string id, const AtlasSizePx atlas_size, const TileSizePx tile_size)
        : Atlas2D(atlas_size, tile_size), texture_id_(std::move(id)) {};

    auto getTextureId() const -> const std::string & { return texture_id_; };
};


}  // namespace will_engine
