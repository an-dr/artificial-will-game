#include <print>

#include "WillEngine/Game.hpp"


using namespace will_engine;

auto main(int argc, char* argv[]) -> int
{
    auto game = Game("Artificial Will");

    auto player_tex = game.loadTexture("player.png", "assets/robot_william.png");
    auto box_tex = game.loadTexture("box.png", "assets/box.png");

    auto world = std::make_unique<World>();
    auto box = world->addEntity({10, 10, 0, 64, 64, 0});

    game.loadWorld(world);

    // ... some init
    return game.start();
}
