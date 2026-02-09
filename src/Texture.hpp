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


namespace engine {
    class Texture {
    public:
        Texture(const std::string & file_path) {
            SDL_Surface *surface = IMG_LoadTexture(file_path.c_str());


        }
    };
}
