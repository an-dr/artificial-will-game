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
#include "../TileMap.hpp"
#include "../entity_components/ComponentGeometry.hpp"
#include "../entity_components/ComponentTexture.hpp"
#include "AssetManager.hpp"
#include "BaseSystem.hpp"


namespace will_engine {

template <typename TileType = int>
class Rendering : public BaseSystem {
    AssetManager *assets_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    const TileMap<TileType> *tile_map_ = nullptr;

    static constexpr int BACKGROUND_R = 0;
    static constexpr int BACKGROUND_G = 0;
    static constexpr int BACKGROUND_B = 0;
    static constexpr int BACKGROUND_A = 255;


    auto draw_tilemap() const -> void {
        if (!tile_map_ || tile_map_->getTextureName().empty()) return;

        SDL_Texture *tileset = assets_->getTexture(tile_map_->getTextureName());
        if (!tileset) return;

        // How many tile columns/rows fit across the tileset image?
        int tileset_w = 0, tileset_h = 0;
        SDL_QueryTexture(tileset, nullptr, nullptr, &tileset_w, &tileset_h);
        const auto src_tile  = tile_map_->getTileSize();        // tile size in the texture
        const auto dst_tile  = tile_map_->getDisplayTileSize(); // tile size on screen
        const int  tileset_cols = tileset_w / src_tile.x;
        const int  tileset_rows = tileset_h / src_tile.y;
        const int  max_index    = tileset_cols * tileset_rows - 1;

        const auto map_size = tile_map_->getSize();
        for (int row = 0; row < map_size.y; row++) {
            for (int col = 0; col < map_size.x; col++) {
                const int tile_index = tile_map_->getTile(col, row);

                if (tile_index < 0 || tile_index > max_index) continue;  // skip invalid tile

                // Where in the tileset is this tile? (row-major grid)
                SDL_Rect src = {
                    .x = (tile_index % tileset_cols) * src_tile.x,
                    .y = (tile_index / tileset_cols) * src_tile.y,
                    .w = src_tile.x,
                    .h = src_tile.y,
                };

                // Where on screen should it be drawn?
                SDL_Rect dst = {
                    .x = col * dst_tile.x,
                    .y = row * dst_tile.y,
                    .w = dst_tile.x,
                    .h = dst_tile.y,
                };

                SDL_RenderCopy(renderer_, tileset, &src, &dst);
            }
        }
    }

    auto draw_static(const ComponentGeometry *geo, const ComponentTexture *tex,
                     SDL_Texture *sdl_tex) const -> void {
        SDL_Rect texture_frame = {.x = tex->frame_position.x,
                                  .y = tex->frame_position.y,
                                  .w = tex->frame_size.x,
                                  .h = tex->frame_size.y};
        SDL_Rect location_frame = {.x = static_cast<int>(geo->x), .y = static_cast<int>(geo->y),
                                   .w = geo->size_x, .h = geo->size_y};
        SDL_RenderCopy(renderer_, sdl_tex, &texture_frame, &location_frame);
    }

    static auto process_animation(float dt, ComponentTexture *tex) -> void {
        if (tex->frames_total < 2)
            return;

        // calculate frames
        float d_secs = dt / 1000.0f;
        auto d_frames = tex->fps * d_secs;

        // advance and wrap frame_current into [0, frames_total)
        tex->frame_current = fmod(tex->frame_current + d_frames,
                                  static_cast<float>(tex->frames_total));

        // round to nearest int
        int current_frame = static_cast<int>(tex->frame_current);

        // update the frame position (horizontal sprite sheet, y stays 0)
        tex->frame_position.x = current_frame * tex->frame_size.x;
        tex->frame_position.y = 0;
    }


    /// Start new frame
    auto startFrame() const -> void {
        SDL_SetRenderDrawColor(renderer_, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
        SDL_RenderClear(renderer_);
    }


    auto completeFrame() const -> void { SDL_RenderPresent(renderer_); }

public:
    Rendering(SDL_Renderer *renderer, AssetManager *asset_manager) : BaseSystem() {
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
            auto textures = getRegistry()->view<ComponentTexture>();
            for (auto entity : textures) {
                auto &tex = textures.get<ComponentTexture>(entity);

                auto sdl_tex = assets_->getTexture(tex.name);
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
} // namespace will_engine