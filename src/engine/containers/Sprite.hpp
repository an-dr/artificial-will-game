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
#include <memory>
#include <utility>
#include "../containers/Geometry.hpp"

namespace will_engine {

using SpiteHitbox = glm::ivec2;

struct SpriteAnimationDescriptor {
    unsigned int fps = 0;
    unsigned int frame_count = 0;  // 0 = use all frames in atlas
};

class Sprite {
    Atlas2D atlas_;
    std::optional<SpiteHitbox> hitbox_;
    std::optional<SpriteAnimationDescriptor> animation_;
    // TODO: move ComponentSpriteRendering components specific the animation here

public:
    explicit Sprite(Atlas2D atlas, std::optional<SpiteHitbox> hitbox = std::nullopt)
        : atlas_(std::move(atlas)), hitbox_(hitbox) {};
    Sprite(Atlas2D atlas, std::optional<SpiteHitbox> hitbox,
           std::optional<SpriteAnimationDescriptor> animation)
        : atlas_(std::move(atlas)), hitbox_(hitbox), animation_(animation) {};

    [[nodiscard]] auto getAnimation() const -> const std::optional<SpriteAnimationDescriptor> & {
        return animation_;
    }
    [[nodiscard]] auto getAtlas() -> Atlas2D & { return atlas_; }
    [[nodiscard]] auto getAtlas() const -> const Atlas2D & { return atlas_; }
    [[nodiscard]] auto getHitbox() const -> const std::optional<SpiteHitbox> & { return hitbox_; }
    [[nodiscard]] auto isAnimation() const { return animation_.has_value(); }
};

using SpriteSharedPtr = std::shared_ptr<const Sprite>;

}  // namespace will_engine
