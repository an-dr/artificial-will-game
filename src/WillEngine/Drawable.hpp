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

namespace will_engine {
    enum class DrawableType {
        Static,
        Animation
    };

    class Drawable {
        SDL_Texture *texture_ = nullptr;
        SDL_Rect texture_frame_ = {0};
        SDL_Rect location_frame_ = {0};

    protected:
        DrawableType type_ = DrawableType::Static;

    public:
        friend class Renderer;

        Drawable(SDL_Texture *texture, const Size &size, const Position &position)
            : texture_(texture) {
            int w = 0;
            int h = 0;
            SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
            texture_frame_ = SDL_Rect{0, 0, w, h};
            location_frame_ = SDL_Rect{position.x, position.y, size.width_x, size.height_y};
        }

        virtual ~Drawable() = default;


        void setPosition(const int x, const int y) {
            location_frame_.x = x;
            location_frame_.y = y;
        }

        void setSize(const int w, const int h) {
            location_frame_.w = w;
            location_frame_.h = h;
        }

        void setFrameAtTexture(const int x, const int y) {
            texture_frame_.x = x;
            texture_frame_.y = y;
        }

        void setFrameAtTexture(const int x, const int y, const int w, const int h) {
            texture_frame_.x = x;
            texture_frame_.y = y;
            texture_frame_.w = w;
            texture_frame_.h = h;
        }

        virtual void draw(SDL_Renderer *renderer) {
            SDL_RenderCopy(renderer, texture_, &texture_frame_, &location_frame_);
        }
    };
}
