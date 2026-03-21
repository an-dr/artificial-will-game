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
#include <SDL.h>
#include <entt/entt.hpp>
#include "../GpuAssetManager.hpp"
#include "../world/TileMap.hpp"
#include "../world/entity_components/ComponentGeometry.hpp"
#include "../world/entity_components/ComponentSprite.hpp"
#include "BaseSystem.hpp"


namespace will_engine {

template <typename TileType = int>
class Rendering : public BaseSystem {
    GpuAssetManager *assets_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    const TileMap<TileType> *tile_map_ = nullptr;

    static constexpr int BACKGROUND_R = 0;
    static constexpr int BACKGROUND_G = 0;
    static constexpr int BACKGROUND_B = 0;
    static constexpr int BACKGROUND_A = 255;


    auto draw_tilemap() const -> void {
        if (!tile_map_ || tile_map_->getTextureName().empty())
            return;

        SDL_Texture *tileset = assets_->getTexture(tile_map_->getTextureName());
        if (!tileset)
            return;

        const auto &atlas = tile_map_->getAtlas();

        for (int i = 0; i < tile_map_->getMapTileCount(); i++) {
            const auto src_rect = atlas.getTile(tile_map_->getTileType(i));
            if (!src_rect.has_value())
                continue;

            const auto dst_rect = tile_map_->getScreenRect(i);

            SDL_Rect src = {src_rect->position.x, src_rect->position.y,  //
                            src_rect->size.x, src_rect->size.y};
            SDL_Rect dst = {dst_rect.position.x, dst_rect.position.y,  //
                            dst_rect.size.x, dst_rect.size.y};

            SDL_RenderCopy(renderer_, tileset, &src, &dst);
        }
    }

    auto draw_static(const ComponentGeometry *geo, const ComponentSprite *sprite,
                     SDL_Texture *sdl_tex) const -> void {
        auto tile = sprite->atlas.getTile(sprite->getFrameInt());
        if (!tile.has_value()) {
            throw std::runtime_error("Sprite map does not have that frame (frame > tiles_max).`");
        }

        SDL_Rect texture_frame = {.x = tile.value().position.x,
                                  .y = tile.value().position.y,
                                  .w = tile.value().size.x,
                                  .h = tile.value().size.y};
        SDL_Rect location_frame = {.x = static_cast<int>(geo->x),
                                   .y = static_cast<int>(geo->y),
                                   .w = geo->size_x,
                                   .h = geo->size_y};
        SDL_RenderCopy(renderer_, sdl_tex, &texture_frame, &location_frame);
    }

    static auto process_animation(float dt, ComponentSprite *sprite) -> void {
        if (sprite->type == SpriteType::Static)
            return;

        // calculate frames
        float d_secs = dt / 1000.0f;
        auto d_frames = sprite->fps * d_secs;

        sprite->bumpFrame(d_frames);
    }


    /// Start new frame
    auto startFrame() const -> void {
        SDL_SetRenderDrawColor(renderer_, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
        SDL_RenderClear(renderer_);
    }


    auto completeFrame() const -> void { SDL_RenderPresent(renderer_); }

public:
    Rendering(SDL_Renderer *renderer, GpuAssetManager *asset_manager) : BaseSystem() {
        if (renderer == nullptr) {
            throw std::runtime_error("renderer_ == nullptr");
        }

        if (asset_manager == nullptr) {
            throw std::runtime_error("asset_manager == nullptr");
        }

        assets_ = asset_manager;
        renderer_ = renderer;
    }

    auto setTileMap(const TileMap<TileType> *tile_map) { tile_map_ = tile_map; }

    auto process(const uint64_t dt_ms) const -> void {
        startFrame();


        // Render tiles
        draw_tilemap();

        // Render objects
        if (isRegisterSet()) {
            auto textures = getRegistry()->view<ComponentSprite>();
            for (auto entity : textures) {
                auto &tex = textures.get<ComponentSprite>(entity);

                auto sdl_tex = assets_->getTexture(tex.atlas.getId());
                if (!sdl_tex) {
                    throw std::runtime_error("texture is not found!");
                }

                auto geo = getRegistry()->try_get<ComponentGeometry>(entity);
                if (!geo) {
                    throw std::runtime_error("entity does not exist!");
                }

                process_animation(dt_ms, &tex);
                draw_static(geo, &tex, sdl_tex);
            }
        }

        completeFrame();
    }
};
}  // namespace will_engine