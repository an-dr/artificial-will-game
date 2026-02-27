#include <print>

#include "WillEngine/Game.hpp"
#include "WillEngine/entity_components/ComponentTexture.hpp"


using namespace will_engine;

auto main(int argc, char* argv[]) -> int
{
    auto game = Game("Artificial Will");

    auto player_tex = game.loadTexture("player.png", "assets/robot_william.png");
    auto box_tex = game.loadTexture("box.png", "assets/box.png");

    auto world = std::make_unique<World>();
    auto box = world->addEntity({10, 10, 0, 64, 64, 0});
    auto box2 = world->addEntity({100, 100, 0, 64, 64, 0});
    world->addComponent<ComponentTexture>(box, box_tex, Point2d{0, 0}, Point2d{64, 64});
    world->addComponent<ComponentTexture>(box2, box_tex, Point2d{0, 0}, Point2d{64, 64});

    game.loadWorld(world);

    // ... some init
    return game.start();
}
