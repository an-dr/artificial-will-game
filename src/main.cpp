#include <print>

#include "WillEngine/Game.hpp"
#include "WillEngine/entity_components/ComponentCollider.hpp"
#include "WillEngine/entity_components/ComponentPlayer.hpp"
#include "WillEngine/entity_components/ComponentTexture.hpp"


using namespace will_engine;

auto main(int argc, char *argv[]) -> int {
    auto game = Game("Artificial Will");

    auto player_tex = game.loadTexture("player.png", "assets/robot_william.png");
    auto box_tex = game.loadTexture("box.png", "assets/box.png");

    auto world = std::make_unique<World>();


    world->add(
        ComponentGeometry{.x = 10, .y = 10, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentTexture{box_tex, Point2d{.x = 0, .y = 0}, Size3d{.x = 64, .y = 64}},
        ComponentCollider{40, 56, true}
        );

    world->add(
        ComponentGeometry{.x = 200, .y = 100, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentTexture{box_tex, Point2d{.x = 0, .y = 0}, Size3d{.x = 64, .y = 64}},
        ComponentCollider{40, 56, true}
        );

    world->add(
        ComponentGeometry{.x = 300, .y = 100, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentTexture{box_tex, Point2d{.x = 0, .y = 0}, Size3d{.x = 64, .y = 64}},
        ComponentCollider{40, 56, true}
        );

    world->addPlayer(
        "Player One",
        ComponentGeometry{.x = 200, .y = 200, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentTexture{player_tex, Point2d{.x = 0, .y = 0},
                         Size3d{.x = 64, .y = 64}, 4u, 0.0f, 8u},
        ComponentCollider{40, 56});

    game.loadWorld(world);

    return game.start();
}