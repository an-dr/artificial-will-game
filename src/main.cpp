#include <SDL.h>
#include <SDL_image.h>  // Add this
#include <iostream>
#include <print>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

void init_logging() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("game.log", true);

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("game", sinks.begin(), sinks.end());

    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug); // or info/warn for release
    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
}

int main(int argc, char *argv[]) {
    init_logging();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Artificial Will",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }


    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1, // first supporting driver
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    }

    SDL_Surface *surface = IMG_Load("assets/robot_william.png");
    if (!surface) {
        std::println("No Image");
        // Handle error: IMG_GetError()
    }

    // Convert to GPU texture
    SDL_Texture *spritesheet = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free CPU copy immediately

    bool running = true;

    int x = 100;
    int y = 100;
    int frame_width = 64;
    int frame_height = 64;

    // Game loop
    int current_frame = 0;
    int frame_counter = 0;
    const int frames_per_anim = 18;  // Change frame every 6 game loops

    int w, h;
    SDL_QueryTexture(spritesheet, nullptr, nullptr, &w, &h);


    spdlog::info("Game started");
    while (running) {
        // Update
        frame_counter++;
        if (frame_counter >= frames_per_anim) {
            current_frame = (current_frame + 1) % 4;  // 0,1,2,3,0,1,2,3...
            frame_counter = 0;
        }

        // 1. Handle input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // 2. Update game state
        // ... your logic ...

        // 3. Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
        SDL_RenderClear(renderer); // Clear backbuffer

        // Draw sprite at position
        SDL_Rect src = {current_frame * 64, 0, 64, 64};  // Assuming 64px frames
        SDL_Rect dest = {x, y, 64, 64};
        SDL_RenderCopy(renderer, spritesheet, &src, &dest);

        SDL_RenderPresent(renderer); // Swap buffers (vsync waits here)
    }


    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Cleanup complete." << std::endl;
    return 0;
}
