#include "game/level_one.hpp"
#include "game/player.hpp"

auto main(int argc, char *argv[]) -> int {
    auto game = Game("Artificial Will");

    // Load textures
    auto player_tex = game.loadTexture("player.png", "assets/robot_william.png");
    auto box_tex = game.loadTexture("box.png", "assets/box.png");
    auto tiles = game.loadTexture(
        "tiles.png", "assets/Pixel Art Top Down - Basic v1.2.3/Texture/TX Tileset Grass.png");

    // Build the world
    auto world = std::make_unique<World<int>>();
    build_tile_map(*world, tiles);
    build_boxes(*world, box_tex);
    build_player(game, *world, player_tex);

    game.loadWorld(world);

    // Start
    return game.start();
}
