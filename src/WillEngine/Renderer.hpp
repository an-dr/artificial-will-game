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
#include "entity_components/Drawable.hpp"
#include "Window.hpp"

namespace will_engine
{
    class Renderer
    {
        SDL_Renderer* renderer_;

    public:
        explicit Renderer(const Window& window)
        {
            renderer_ = SDL_CreateRenderer(
                window.getSdlWindow(),
                -1, // first supporting driver
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );

            if (renderer_ == nullptr)
            {
                SDL_Quit();
                throw std::runtime_error("Cannot create a renderer");
            }
        }

        ~Renderer()
        {
            SDL_DestroyRenderer(renderer_);
        }

        /// Start new frame
        auto startFrame() const -> void
        {
            SDL_SetRenderDrawColor(getSdlRenderer(), 0, 0, 0, 255);
            SDL_RenderClear(getSdlRenderer());
        }

        auto draw(Drawable& drawable) const -> void
        {
            drawable.draw(getSdlRenderer());
        }

        auto completeFrame() const -> void
        {
            SDL_RenderPresent(getSdlRenderer());
        }

        [[nodiscard]] auto getSdlRenderer() const -> SDL_Renderer*
        {
            return renderer_;
        }
    };
}
