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
#include <cmath>
#include "../../containers/Sprite.hpp"


namespace will_engine {


struct ComponentSpriteRendering {
    SpriteSharedPtr sprite;
    float frame_float = 0.0f;  // float to progress smoothly

    auto bumpFrame(float dframe) {
        if (!isAnimated())
            return;

        const auto &atlas = sprite->getAtlas();
        const auto &animation = sprite->getAnimation().value();
        auto tiles = atlas.getAtlasSizeTiles();
        auto total_frames = static_cast<float>(animation.frame_count > 0 ? animation.frame_count
                                                                         : tiles.x * tiles.y);
        frame_float = fmod(frame_float + dframe, total_frames);
    }
    [[nodiscard]] auto getFrameInt() const { return isAnimated() ? static_cast<int>(frame_float) : 0; }
    [[nodiscard]] auto getFrameCount() const -> unsigned int {
        if (!sprite)
            return 0;

        const auto &animation = sprite->getAnimation();
        if (!animation.has_value())
            return 1;

        auto tiles = sprite->getAtlas().getAtlasSizeTiles();
        return animation->frame_count > 0 ? animation->frame_count
                                          : static_cast<unsigned int>(tiles.x * tiles.y);
    }
    [[nodiscard]] auto isAnimated() const -> bool {
        if (!sprite || !sprite->getAnimation().has_value())
            return false;

        return sprite->getAnimation()->fps > 0 && getFrameCount() > 1;
    }
    auto setSprite(SpriteSharedPtr new_sprite, bool reset_frame = true) -> void {
        if (sprite == new_sprite)
            return;

        sprite = std::move(new_sprite);
        if (reset_frame) {
            frame_float = 0.0f;
        }
    }
};


}  // namespace will_engine
