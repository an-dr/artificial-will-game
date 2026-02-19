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
#include "Drawable.hpp"


namespace will_engine
{
    class Animation : public Drawable
    {
        int frames_ = 0;
        int current_a_frame = 0;
        int frame_counter_ = 0;
        int frames_per_a_frame = 18;
        Size animation_frame_size = Size(0, 0);

    public:
        Animation(SDL_Texture* texture, const int frames, const int fps, const Size frame_size,
                  const Size& size, const Position& position) : Drawable(texture, size, position),
                                                                frames_(frames),
                                                                frames_per_a_frame(fps),
                                                                animation_frame_size(frame_size)
        {
            type_ = DrawableType::Animation;
            setFrameAtTexture(0, 0, animation_frame_size.width_x, animation_frame_size.height_y);
        }

        auto draw(SDL_Renderer* renderer) -> void override
        {
            frame_counter_++;
            if (frame_counter_ >= frames_per_a_frame)
            {
                current_a_frame = (current_a_frame + 1) % frames_;
                frame_counter_ = 0;
            }
            setFrameAtTexture(current_a_frame * animation_frame_size.width_x, 0);
            Drawable::draw(renderer);
        }
    };
}
