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
#include <SDL_image.h>
#include <stdexcept>
#include <string>
#include <unordered_map>


namespace will_engine {
    class AssetManager {
        SDL_Renderer *renderer_;
        std::unordered_map<std::string, SDL_Texture *> textures_;

    public:
        explicit AssetManager(SDL_Renderer *renderer) : renderer_(renderer) {
        }

        ~AssetManager() {
            for (const auto &[name, texture]: textures_) {
                SDL_DestroyTexture(texture);
            }
            textures_.clear();
        }

        auto loadTexture(const std::string &name, const std::string &file_path) -> std::string {
            if (const auto it = textures_.find(name); it != textures_.end()) {
                return name; // Already loaded
            }

            SDL_Texture *t = IMG_LoadTexture(renderer_, file_path.c_str());
            if (!t) {
                throw std::runtime_error("No image");
            }

            textures_[name] = t;
            return name;
        }

        auto getTexture(const std::string &name) -> SDL_Texture * {
            if (const auto it = textures_.find(name); it != textures_.end()) {
                return it->second;
            }
            return nullptr;
        }
    };
}
