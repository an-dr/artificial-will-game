#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Artificial Will - Hello World",
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

    std::cout << "SDL2 window created successfully!" << std::endl;
    std::cout << "Press any key to close..." << std::endl;

    SDL_Delay(5000);  // Show for 5 seconds

    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Cleanup complete." << std::endl;
    return 0;
}
