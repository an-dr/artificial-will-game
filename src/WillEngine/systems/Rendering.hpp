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
#include <SDL.h>
#include <entt/entt.hpp>
#include "../entity_components/ComponentDrawable.hpp"


namespace will_engine {
    class Rendering {
        SDL_Renderer *renderer_;
        entt::registry *registry_; // All entities/components here

        static constexpr int BACKGROUND_R = 0;
        static constexpr int BACKGROUND_G = 0;
        static constexpr int BACKGROUND_B = 0;
        static constexpr int BACKGROUND_A = 255;

    public:
        Rendering(entt::registry &registry, SDL_Renderer *renderer)
            : registry_(&registry) {
            if (renderer == nullptr) {
                throw std::runtime_error("renderer_ == nullptr");
            }
            renderer_ = renderer;
        }

        /// Start new frame
        auto startFrame() const -> void {
            SDL_SetRenderDrawColor(renderer_, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
            SDL_RenderClear(renderer_);
        }


        auto draw() const -> void {
            auto view = renderer_.view<ComponentDrawable>();
            for (auto entity: view) {

                // drawable.draw(getSdlRenderer());
            }
        }

        auto completeFrame() const -> void {
            SDL_RenderPresent(renderer_);
        }
    };
}

