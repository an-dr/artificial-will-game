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

#include <string>
#include <SDL_image.h>
#include "../Window.hpp"


namespace will_engine {
    class Texture {
        SDL_Texture *texture_ = nullptr;

    public:
        Texture(const std::string &file_path, const Window &window) {
            SDL_Texture *t = IMG_LoadTexture(window.getSdlRenderer(), file_path.c_str());
            if (!t) {
                throw std::runtime_error("No image");
            }
            texture_ = t;
        }

        ~Texture() {
            SDL_DestroyTexture(texture_);
        }

        auto getSdlTexture() -> SDL_Texture * {
            return texture_;
        }
    };
}
