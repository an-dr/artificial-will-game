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

namespace will_engine
{
    class Window
    {
        SDL_Window* window_;

    public:
        Window(const std::string& title, const int width, const int height)
        {
            window_ = SDL_CreateWindow(title.c_str(),
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width, height,
                                       SDL_WINDOW_SHOWN);
            if (window_ == nullptr)
            {
                SDL_Quit();
                throw std::runtime_error("Cannot create a window");
            }
        }

        ~Window()
        {
            SDL_DestroyWindow(window_);
        }

        [[nodiscard]] auto getSdlWindow() const -> SDL_Window* { return window_; }
    };
} // will_engine
