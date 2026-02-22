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

/*

✅ Window owns Renderer - SDL2 requires this, destroyed together
✅ Systems are objects - structs with state, not free functions
✅ NO system inheritance - no ISystem base class, no polymorphism
✅ Different system signatures - each takes what it needs
✅ Game::run() encapsulates loop - main.cpp just creates Game and calls run()
✅ World owns registry - all entity data lives here
✅ Components are POD - plain structs, no methods that modify state

main.cpp
cppint main(int argc, char* argv[]) {
    try {
        Game game;
        game.run();  // That's it
    } catch (const std::exception& e) {
        // Error handling
        return 1;
    }
    return 0;
}
 *
 */

#pragma once
#include <memory>
#include "Window.hpp"
#include "systems/Input.hpp"
#include "ulog.h"
#include <SDL.h>
#include <SDL_image.h>  // Add this
#include <iostream>
#include <print>
#include <box2d/box2d.h>
#include "WillEngine/Window.hpp"
#include "WillEngine/entity_components/Animation.hpp"
#include "WillEngine/entity_components/Texture.hpp"
#include "WillEngine/World.hpp"
#include "WillEngine/systems/Input.hpp"
#include "ulog.h"


namespace will_engine {
    class Game {
        Window window_;
        Input sys_input_;

    public:
        Game() : window_(Window("Artificial Will", 800, 600)) {
            ulog_topic_add("Game", ULOG_OUTPUT_ALL, ULOG_LEVEL_DEBUG);

            if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                ulog_error("SDL_Init Error: {}", SDL_GetError());
            }

            if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
                ulog_error("IMG_Init Error: {}", IMG_GetError());
            }
        }

        auto start() -> int {
            const auto robot_william_png = std::make_shared<Texture>("assets/robot_william.png", window_);
            const auto box_png = std::make_shared<Texture>("assets/box.png", window_);

            std::vector<std::shared_ptr<Drawable> > drawables;

            const auto box = std::make_shared<Drawable>(box_png->getSdlTexture(), Size(64, 64), Position(200, 200));
            drawables.push_back(box);

            auto player_pos = Position(100, 100);
            const auto player = std::make_shared<Animation>(robot_william_png->getSdlTexture(), 4, 18, Size(64, 64),
                                                            Size(64, 64),
                                                            player_pos);
            drawables.push_back(player);

            auto input = will_engine::Input();

            ulog_t_info("Game", "Game started");
            bool running = true;
            while (running) {
                // Handle input
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                    }
                }

                // Render
                window_.startFrame();

                for (auto &d: drawables) {
                    window_.draw(*d);
                }

                if (auto ev = input.get()) {
                    switch (ev.value()) {
                        case InputEvent::Down:
                            player_pos.y += 2; // Also fixed direction - see below
                            break;
                        case InputEvent::Up:
                            player_pos.y -= 2;
                            break;
                        case InputEvent::Left:
                            player_pos.x -= 2;
                            break;
                        case InputEvent::Right:
                            player_pos.x += 2;
                            break;
                        case InputEvent::None:
                            break;
                    }
                }

                player->setPosition(player_pos.x, player_pos.y);

                window_.completeFrame();
            }


            SDL_Quit();

            ulog_t_info("Game", "Game is closed");


            return 0;
        }
    };
} // will_engine
