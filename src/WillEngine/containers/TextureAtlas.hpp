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
#include "../containers/Geometry.hpp"


namespace will_engine {

class TextureAtlas : public Atlas2D {
    std::string name_id;

public:
    TextureAtlas(const std::string &id, const AtlasSize atlas_size, const TileSize tile_size)
        : Atlas2D(atlas_size, tile_size), name_id(id) {};

    auto getId() -> std::string { return name_id; };
};


}  // namespace will_engine