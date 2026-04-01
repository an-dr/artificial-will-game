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
#include "../../containers/Sprite.hpp"


namespace will_engine {


struct ComponentSpriteRendering {
    Sprite sprite;
    float frame_float;  // float to progress smoothly
    SpriteType type = SpriteType::Static;
    unsigned int fps;
    unsigned int frame_count = 0;  // 0 = use all frames in atlas

    auto bumpFrame(float dframe) {
        auto tiles = sprite.getAtlasSizeTiles();
        auto total_frames = static_cast<float>(frame_count > 0 ? frame_count : tiles.x * tiles.y);
        frame_float = fmod(frame_float + dframe, total_frames);
    }
    auto getFrameInt() const { return static_cast<int>(frame_float); }
};


}  // namespace will_engine