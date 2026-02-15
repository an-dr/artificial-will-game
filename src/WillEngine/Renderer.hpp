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
#include <optional>
#include <SDL.h>
#include <stdexcept>

#include "Drawable.hpp"

namespace will_engine {
    class Window;

    class Renderer {
        SDL_Renderer *renderer_;

    public:
        explicit Renderer(const Window &window) {
            renderer_ = SDL_CreateRenderer(
                window.window_,
                -1, // first supporting driver
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );

            if (renderer_ == nullptr) {
                SDL_Quit();
                throw std::runtime_error("Cannot create a renderer");
            }
        }

        /// Start new frame
        void startFrame() const {
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderClear(renderer_);
        }


        void copyTexture(const SDL_Texture *texture,
                         const std::optional<SDL_Rect> &src,
                         const std::optional<SDL_Rect> &dest) const {
            const SDL_Rect *s = src ? &(*src) : nullptr;
            const SDL_Rect *d = dest ? &(*dest) : nullptr;

            SDL_RenderCopy(renderer_, const_cast<SDL_Texture *>(texture), s, d);
        }

        void draw(Drawable &drawable) const {
            drawable.draw(renderer_);
        }

        void completeFrame() const {
            SDL_RenderPresent(renderer_);
        }

        [[nodiscard]] SDL_Renderer *getRenderer() const {
            return renderer_;
        }
    };
}
