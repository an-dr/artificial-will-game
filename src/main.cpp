#include <SDL.h>
#include <SDL_image.h>  // Add this
#include <iostream>
#include <print>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "WillEngine/Window.hpp"
#include "WillEngine/entity_components/Animation.hpp"
#include "WillEngine/Renderer.hpp"
#include "WillEngine/entity_components/Texture.hpp"
#include "WillEngine/World.hpp"
#include "WillEngine/systems/Input.hpp"

void init_logging() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("game.log", true);

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("game", sinks.begin(), sinks.end());

    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug); // or info/warn for release
    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
}

using namespace will_engine;

auto main(int argc, char *argv[]) -> int {
    init_logging();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::error("SDL_Init Error: {}", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        spdlog::error("IMG_Init Error");
    }

    World w;

    const auto window = Window("Artificial Will", 800, 600);
    const auto renderer = Renderer(window);
    const auto robot_william_png = std::make_shared<Texture>("assets/robot_william.png", renderer);
    const auto box_png = std::make_shared<Texture>("assets/box.png", renderer);

    std::vector<std::shared_ptr<Drawable> > drawables;

    const auto box = std::make_shared<Drawable>(box_png->getSdlTexture(), Size(64, 64), Position(200, 200));
    drawables.push_back(box);

    auto player_pos = Position(100, 100);
    const auto player = std::make_shared<Animation>(robot_william_png->getSdlTexture(), 4, 18, Size(64, 64),
                                                    Size(64, 64),
                                                    player_pos);
    drawables.push_back(player);

    auto input = will_engine::Input();

    spdlog::info("Game started");
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
        renderer.startFrame();

        for (auto &d: drawables) {
            renderer.draw(*d);
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

        renderer.completeFrame();
    }


    SDL_Quit();

    spdlog::info("Game is closed");
    return 0;
}
