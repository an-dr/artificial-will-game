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


namespace will_engine {

class TextureAtlas : public Atlas2D {
    std::string name_id;

public:
    TextureAtlas(std::string id, const AtlasSizePx atlas_size, const TileSizePx tile_size)
        : Atlas2D(atlas_size, tile_size), name_id(std::move(id)) {};

    auto getId() const -> const std::string & { return name_id; };
};


}  // namespace will_engine
