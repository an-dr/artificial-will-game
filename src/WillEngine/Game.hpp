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
#include <iostream>
#include <memory>
#include <print>
#include <utility>
#include <SDL.h>
#include <SDL_image.h>  // Add this
#include <box2d/box2d.h>
#include "Window.hpp"
#include "World.hpp"
#include "systems/AssetManager.hpp"
#include "systems/Input.hpp"
#include "systems/Rendering.hpp"
#include "ulog.h"


namespace will_engine {
class Game {
    std::string name_;
    std::unique_ptr<Window> window_;
    std::unique_ptr<Input> sys_input_;
    std::unique_ptr<Rendering> sys_rendering_;
    std::unique_ptr<AssetManager> assets_;
    std::unique_ptr<World> world_;
    uint64_t last_update_ms_ = 0;

    // update time for systems, return delta
    auto update_time() -> uint64_t {
        auto now = SDL_GetTicks64();
        auto delta = now - last_update_ms_;
        last_update_ms_ = now;
        return delta;
    }

public:
    Game(std::string name = "Game") : name_(std::move(name)) {
        ulog_topic_add("Game", ULOG_OUTPUT_ALL, ULOG_LEVEL_DEBUG);

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            ulog_error("SDL_Init Error: {}", SDL_GetError());
        }

        if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
            ulog_error("IMG_Init Error: {}", IMG_GetError());
        }

        // Now construct members that depend on SDL
        window_ = std::make_unique<Window>(name_, 800, 600);
        assets_ = std::make_unique<AssetManager>(window_->getSdlRenderer());
        sys_rendering_ = std::make_unique<Rendering>(window_->getSdlRenderer(), assets_.get());
        sys_input_ = std::make_unique<Input>();
    }

    auto loadTexture(const std::string &name, const std::string &file_path) -> std::string {
        return assets_->loadTexture(name, file_path);
    }

    auto loadWorld(std::unique_ptr<World> &world) -> void {
        world_.reset();  // destroy old world!!!
        world_ = std::move(world);
        sys_rendering_->setRegistry(world_->getRegistry());
        sys_input_->setRegistry(world_->getRegistry());
        sys_input_->setPhysicsWorld(world_->getPhysicsWorld());
    }

    auto start() -> int {
        ulog_t_info("Game", "%s started", name_.c_str());

#if 0
        auto input = will_engine::Input();

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
#endif  // if 0

        bool running = true;

        update_time();
        while (running) {
            auto dt_ms = update_time();

            // Handle input
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            sys_input_->process(dt_ms / 1000.0f, World::PPM);
            sys_rendering_->process(dt_ms);
        }


        SDL_Quit();

        ulog_t_info("Game", "%s closed", name_.c_str());


        return 0;
    }
};

}  // namespace will_engine