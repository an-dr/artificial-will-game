#include "game/level_one.hpp"

auto main(int argc, char *argv[]) -> int {
    auto game = Game("Artificial Will");

    auto l1 = build_level_one(game);
    game.loadWorld(l1);

    return game.start();
}
