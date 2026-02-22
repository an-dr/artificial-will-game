#include <print>

#include "WillEngine/Game.hpp"


using namespace will_engine;

auto main(int argc, char *argv[]) -> int {
    auto game = Game();

    // ... some init
    return game.start();
}
