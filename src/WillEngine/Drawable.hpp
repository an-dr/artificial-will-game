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
#include "Position.hpp"
#include "Size.hpp"

namespace will_engine
{
    enum class DrawableType
    {
        Static,
        Animation
    };

    class Drawable
    {
        SDL_Texture* texture_ = nullptr;
        SDL_Rect texture_frame_ = {};
        SDL_Rect location_frame_ = {};

    protected:
        DrawableType type_ = DrawableType::Static;

    public:
        friend class Renderer;

        Drawable(SDL_Texture* texture, const Size& size, const Position& position)
            : texture_(texture)
        {
            int w = 0;
            int h = 0;
            SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
            texture_frame_ = SDL_Rect{.x = 0, .y = 0, .w = w, .h = h};
            location_frame_ = SDL_Rect{.x = position.x, .y = position.y, .w = size.width_x, .h = size.height_y};
        }


        virtual ~Drawable() = default;


        auto setPosition(const int x, const int y) -> void
        {
            location_frame_.x = x;
            location_frame_.y = y;
        }

        auto setSize(const int w, const int h) -> void
        {
            location_frame_.w = w;
            location_frame_.h = h;
        }

        auto setFrameAtTexture(const int x, const int y) -> void
        {
            texture_frame_.x = x;
            texture_frame_.y = y;
        }

        auto setFrameAtTexture(const int x, const int y, const int w, const int h) -> void
        {
            texture_frame_.x = x;
            texture_frame_.y = y;
            texture_frame_.w = w;
            texture_frame_.h = h;
        }

        virtual auto draw(SDL_Renderer* renderer) -> void
        {
            SDL_RenderCopy(renderer, texture_, &texture_frame_, &location_frame_);
        }
    };
}
