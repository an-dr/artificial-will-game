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
#include <stdexcept>
#include "entity_components/Drawable.hpp"

namespace will_engine {
    class Window {
        SDL_Window *window_;
        SDL_Renderer *renderer_;

    public:
        Window(const std::string &title, const int width, const int height) {
            window_ = SDL_CreateWindow(title.c_str(),
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width, height,
                                       SDL_WINDOW_SHOWN);

            if (window_ == nullptr) {
                SDL_Quit();
                throw std::runtime_error("Cannot create a window");
            }


            renderer_ = SDL_CreateRenderer(window_,
                                           -1, // first supporting driver
                                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );

            if (renderer_ == nullptr) {
                SDL_Quit();
                throw std::runtime_error("Cannot create a renderer");
            }
        }

        ~Window() {
            SDL_DestroyRenderer(renderer_);
            SDL_DestroyWindow(window_);
        }

        /// Start new frame
        auto startFrame() const -> void {
            SDL_SetRenderDrawColor(getSdlRenderer(), 0, 0, 0, 255);
            SDL_RenderClear(getSdlRenderer());
        }

        auto draw(Drawable &drawable) const -> void {
            drawable.draw(getSdlRenderer());
        }

        auto completeFrame() const -> void {
            SDL_RenderPresent(getSdlRenderer());
        }

        [[nodiscard]] auto getSdlWindow() const -> SDL_Window * { return window_; }

        [[nodiscard]] auto getSdlRenderer() const -> SDL_Renderer * {
            return renderer_;
        }
    };
} // will_engine
