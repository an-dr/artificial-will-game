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


namespace will_engine
{
    class Rendering
    {
        SDL_Renderer* renderer_;
        entt::registry* registry_ = nullptr; // All entities/components here

        static constexpr int BACKGROUND_R = 0;
        static constexpr int BACKGROUND_G = 0;
        static constexpr int BACKGROUND_B = 0;
        static constexpr int BACKGROUND_A = 255;

    public:
        Rendering(SDL_Renderer* renderer)
        {
            if (renderer == nullptr)
            {
                throw std::runtime_error("renderer_ == nullptr");
            }
            renderer_ = renderer;
        }

        auto setRegistry(entt::registry* registry) -> void
        {
            registry_ = registry;
        }

        /// Start new frame
        auto startFrame() const -> void
        {
            SDL_SetRenderDrawColor(renderer_, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
            SDL_RenderClear(renderer_);
        }


        auto draw() const -> void
        {
            startFrame();

            if (registry_)
            {
                // for (const auto& [name, texture] : textures_)
                // {
                //     SDL_Rect texture_frame = {};
                //     SDL_Rect location_frame = {};
                //
                //     SDL_RenderCopy(renderer_, texture, &texture_frame, &location_frame);
                // }
            }


            completeFrame();


            // auto view = renderer_.view<ComponentTexture>();
            // for (auto entity: view) {
            //
            //     // drawable.draw(getSdlRenderer());
            // }
        }

        auto completeFrame() const -> void
        {
            SDL_RenderPresent(renderer_);
        }
    };
}
