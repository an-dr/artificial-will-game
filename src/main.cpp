#include <print>

#include "WillEngine/Game.hpp"
#include "WillEngine/entity_components/ComponentCollider.hpp"
#include "WillEngine/entity_components/ComponentPlayer.hpp"
#include "WillEngine/entity_components/ComponentTexture.hpp"


using namespace will_engine;

auto main(int argc, char* argv[]) -> int
{
    auto game = Game("Artificial Will");

    auto player_tex = game.loadTexture("player.png", "assets/robot_william.png");
    auto box_tex = game.loadTexture("box.png", "assets/box.png");

    auto world = std::make_unique<World>();

    // Static boxes
    auto box = world->addEntity({.x = 10, .y = 10, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0});
    world->addComponent<ComponentCollider>(box, 40, 56);
    world->addComponent<ComponentTexture>(box, box_tex, Point2d{.x = 0, .y = 0},
                                          Size3d{.x = 64, .y = 64});

    auto box2 = world->addEntity({.x = 100, .y = 100, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0});
    world->addComponent<ComponentCollider>(box2, 40, 56);
    world->addComponent<ComponentTexture>(box2, box_tex, Point2d{.x = 0, .y = 0},
                                          Size3d{.x = 64, .y = 64});

    // Pushable box
    auto box3 = world->addEntity({.x = 300, .y = 100, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0});
    world->addComponent<ComponentCollider>(box3, 40, 56, true);
    world->addComponent<ComponentTexture>(box3, box_tex, Point2d{.x = 0, .y = 0},
                                          Size3d{.x = 64, .y = 64});

    // Player
    auto player = world->addEntity({.x = 200, .y = 200, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0});
    world->addComponent<ComponentCollider>(player, 40, 56);
    world->addComponent<ComponentTexture>(player, player_tex, Point2d{.x = 0, .y = 0},
                                          Size3d{.x = 64, .y = 64}, 4u, 0.0f, 8u);
    world->addComponent<ComponentPlayer>(player, "Player One", 1U);

    game.loadWorld(world);

    return game.start();
}
