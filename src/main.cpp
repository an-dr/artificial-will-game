#include <SDL.h>
#include <SDL_image.h>  // Add this
#include <iostream>
#include <print>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "WillEngine/Window.hpp"
#include "WillEngine/Animation.hpp"
#include "WillEngine/Renderer.hpp"
#include "WillEngine/Texture.hpp"
#include "WillEngine/Input.hpp"

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

int main(int argc, char *argv[]) {
    init_logging();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    const auto window = Window("Artificial Will", 800, 600);
    const auto renderer = Renderer(window);
    auto robot_william_png = Texture("assets/robot_william.png", renderer);
    auto box_png = Texture("assets/box.png", renderer);

    auto pos = Position(100,100);
    Animation player(robot_william_png.getSdlTexture(), 4, 18, Size(64,64), Size(64, 64), pos);
    Drawable box(box_png.getSdlTexture(), Size(64, 64), Position(200, 200));

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
        renderer.draw(box);
        renderer.draw(player);

        if (auto ev = input.get()) {
            switch (ev.value()) {
                case InputEvent::Down:
                    pos.y += 2; // Also fixed direction - see below
                    break;
                case InputEvent::Up:
                    pos.y -= 2;
                    break;
                case InputEvent::Left:
                    pos.x -= 2;
                    break;
                case InputEvent::Right:
                    pos.x += 2;
                    break;
                case InputEvent::None:
                    break;
            }
        }

        player.setPosition(pos.x, pos.y);

        renderer.completeFrame();
    }


    SDL_Quit();

    spdlog::info("Game is closed");
    return 0;
}
