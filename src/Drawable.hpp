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

namespace engine {
    class Window;
    class Renderer;

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
        friend class Window;
        friend class Renderer;

        Drawable(SDL_Texture *texture, SDL_Rect &position)
            : texture_(texture), location_frame_(position) {
            SDL_QueryTexture(texture, nullptr, nullptr, &texture_frame_.w, &texture_frame_.h);
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

        void setTextureFrame(const int x, const int y) {
            texture_frame_.x = x;
            texture_frame_.y = y;
        }

        void setTextureFrame(const int x, const int y, const int w, const int h) {
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
