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


namespace engine {
    class Animation : public Drawable {
        int frames_ = 0;
        int current_a_frame = 0;
        int frames_per_a_frame = 18;
        int animation_frame_w = 0;
        int animation_frame_h = 0;

    public:
        Animation(SDL_Texture *texture, const int frames, const int fps, const int aframe_w, const int aframe_h,
                  SDL_Rect &position) : Drawable(
                                            texture, position), frames_(frames), frames_per_a_frame(fps),
                                        animation_frame_w(aframe_w),
                                        animation_frame_h(aframe_h) {
            type_ = DrawableType::Animation;
            setTextureFrame(0, 0, animation_frame_w, animation_frame_h);
        }

        void update() {
            current_a_frame++;
            if (current_a_frame >= frames_per_a_frame) {
                current_a_frame = (current_a_frame + 1) % frames_;
                current_a_frame = 0;
            }
            setTextureFrame(current_a_frame * animation_frame_w, 0);
        }
    };
}
