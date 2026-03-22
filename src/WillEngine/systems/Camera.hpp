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
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include "../world/CameraState.hpp"
#include "../world/entity_components/ComponentCameraTarget.hpp"
#include "../world/entity_components/ComponentGeometry.hpp"
#include "BaseSystem.hpp"

namespace will_engine {
class Camera : public BaseSystem {
    glm::vec2 viewport_ = {800.0f, 600.0f};     // basically, window size
    glm::vec2 world_bounds_max = {0.0f, 0.0f};  // (0,0) = no clamping

public:
    Camera() = default;

    static constexpr float smoothing = 5.0f;

    /// Set view rectangle
    auto setViewport(float w, float h) -> void { viewport_ = {w, h}; }


    /// Set boundaries not to move camera beyond
    auto setWorldBounds(float w, float h) -> void { world_bounds_max = {w, h}; }

    auto process(float dt_seconds, CameraState *camera_state) const -> void {
        auto *registry = getRegistry();
        if (!registry || !camera_state)
            return;

        // For each ComponentCameraTarget entity untill the first target to follow
        for (const auto &[entity, camera_target, geometry] :
             registry->view<ComponentCameraTarget, ComponentGeometry>().each()) {
            if (!camera_target.follow)
                continue;

            // The target position is to put the camera in the middle of the viewport
            const glm::vec2 target = {geometry.x - viewport_.x * 0.5f,
                                      geometry.y - viewport_.y * 0.5f};

            // Shift the position until the diff is zero
            auto camera_diff2target = target - camera_state->position;
            camera_state->position += camera_diff2target * smoothing * dt_seconds;

            // Do not move beyond the world
            if (world_bounds_max.x > 0 && world_bounds_max.y > 0) {
                // 0.0 - is the beginning, allowed
                glm::vec2 allowed_min = {0.0f, 0.0f};
                // keep viewport_ inside the world
                glm::vec2 allowed_max = world_bounds_max - viewport_;

                camera_state->position =
                    glm::clamp(camera_state->position, allowed_min, allowed_max);
            }
            break;
        }
    };
};
}  // namespace will_engine