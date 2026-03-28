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
#include <memory>
#include <utility>
#include <SDL.h>
#include <SDL_image.h>
#include "GpuAssetManager.hpp"
#include "Window.hpp"
#include "systems/SystemCamera.hpp"
#include "systems/SystemInput.hpp"
#include "systems/SystemMovementAndCollision.hpp"
#include "systems/SystemRendering.hpp"
#include "systems/SystemState.hpp"
#include "ulog.h"
#include "world/World.hpp"


namespace will_engine {
class Game {
    std::string name_;
    std::unique_ptr<Window> window_;
    std::unique_ptr<SystemInput> sys_input_;
    std::unique_ptr<SystemRendering<int>> sys_rendering_;
    std::unique_ptr<SystemMovementAndCollision> sys_movement_;
    std::unique_ptr<SystemCamera> sys_camera_;
    std::unique_ptr<SystemState> sys_state_;
    std::unique_ptr<GpuAssetManager> gpu_assets_;
    std::unique_ptr<World<int>> world_;
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
        gpu_assets_ = std::make_unique<GpuAssetManager>(window_->getSdlRenderer());
        sys_rendering_ =
            std::make_unique<SystemRendering<int>>(window_->getSdlRenderer(), gpu_assets_.get());
        sys_input_ = std::make_unique<SystemInput>();
        sys_movement_ = std::make_unique<SystemMovementAndCollision>();
        sys_state_ = std::make_unique<SystemState>();
        sys_camera_ = std::make_unique<SystemCamera>();
        sys_camera_->setViewport(800, 600);
    }

    auto loadTexture(const std::string &name, const std::string &file_path) const -> std::string {
        return gpu_assets_->loadTexture(name, file_path);
    }

    auto loadWorld(std::unique_ptr<World<int>> &world) -> void {
        world_.reset();  // destroy old world!!!
        world_ = std::move(world);
        sys_rendering_->setRegistry(world_->getRegistry());
        sys_rendering_->setTileMap(world_->getTileMap());
        sys_input_->setRegistry(world_->getRegistry());
        sys_movement_->setRegistry(world_->getRegistry());
        sys_camera_->setRegistry(world_->getRegistry());
        sys_state_->setRegistry(world_->getRegistry());
        sys_state_->setStateMachineRegistry();

        if (auto *tm = world_->getTileMap()) {
            auto map = tm->getMapSize();
            auto tile = tm->getRenderTileSize();
            sys_camera_->setWorldBounds(map.x * tile.x, map.y * tile.y);
        }
    }

    auto addStateMachine(std::unique_ptr<IStateMachine> state_machine) const {
        sys_state_->add(std::move(state_machine));
        sys_state_->setStateMachineRegistry();
    }

    auto start() -> int {
        ulog_t_info("Game", "%s started", name_.c_str());

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

            sys_input_->process(dt_ms / 1000.0f);
            sys_movement_->process(dt_ms / 1000.0f);
            sys_camera_->process(dt_ms / 1000.0f, world_->getCameraState());
            sys_state_->process();
            sys_rendering_->process(dt_ms, world_->getCameraState());
        }


        SDL_Quit();

        ulog_t_info("Game", "%s closed", name_.c_str());


        return 0;
    }
};

}  // namespace will_engine