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
#include "../entity_components/ComponentObject.hpp"
#include "../entity_components/ComponentTexture.hpp"
#include "AssetManager.hpp"


namespace will_engine {
    class Rendering {
        AssetManager *assets_ = nullptr;
        SDL_Renderer *renderer_ = nullptr;
        entt::registry *registry_ = nullptr; // All entities/components here

        static constexpr int BACKGROUND_R = 0;
        static constexpr int BACKGROUND_G = 0;
        static constexpr int BACKGROUND_B = 0;
        static constexpr int BACKGROUND_A = 255;

    public:
        Rendering(SDL_Renderer *renderer, AssetManager *asset_manager) {
            if (renderer == nullptr) {
                throw std::runtime_error("renderer_ == nullptr");
            }

            if (asset_manager == nullptr) {
                throw std::runtime_error("asset_manager == nullptr");
            }

            assets_ = asset_manager;
            renderer_ = renderer;
        }

        auto setRegistry(entt::registry *registry) -> void {
            registry_ = registry;
        }

        /// Start new frame
        auto startFrame() const -> void {
            SDL_SetRenderDrawColor(renderer_, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
            SDL_RenderClear(renderer_);
        }


        auto draw() const -> void {
            startFrame();

            if (registry_) {
                auto textures = registry_->view<ComponentTexture>();
                for (auto entity: textures) {
                    auto &comp_tex = textures.get<ComponentTexture>(entity);
                    auto texture = assets_->getTexture(comp_tex.name);
                    if (!texture) {
                        throw std::runtime_error("texture is not found!");
                    }

                    SDL_Rect texture_frame = {
                        comp_tex.frame_position.x,
                        comp_tex.frame_position.y,
                        comp_tex.rect_b.x - comp_tex.frame_position.x,
                        comp_tex.rect_b.y - comp_tex.frame_position.y
                    };

                    auto &geo = registry_->get<ComponentObject>(entity);
                    SDL_Rect location_frame = {geo.x, geo.y, geo.size_x, geo.size_y};
                    SDL_RenderCopy(renderer_, texture, &texture_frame, &location_frame);
                }
            }


            completeFrame();


            // auto view = renderer_.view<ComponentTexture>();
            // for (auto entity: view) {
            //
            //     // drawable.draw(getSdlRenderer());
            // }
        }

        auto completeFrame() const -> void {
            SDL_RenderPresent(renderer_);
        }
    };
}
