#include "WillEngine/Game.hpp"
#include "WillEngine/world/entity_components/ComponentCollider.hpp"
#include "WillEngine/world/entity_components/ComponentSprite.hpp"


using namespace will_engine;


auto main(int argc, char *argv[]) -> int {
    auto game = Game("Artificial Will");

    auto player_tex = game.loadTexture("player.png", "assets/robot_william.png");
    auto box_tex = game.loadTexture("box.png", "assets/box.png");
    auto tiles = game.loadTexture(
        "tiles.png", "assets/Pixel Art Top Down - Basic v1.2.3/Texture/TX Tileset Grass.png");

    auto world = std::make_unique<World<int>>();

    auto tile_map = TileMap<int>{ArraySize2D{16, 16}, TileSizePx{64, 64}, 0,
                                 TextureAtlas{tiles, AtlasSizePx{256, 256}, TileSizePx{64, 64}}};
    std::vector<int> tile_descriptor = {};
    tile_descriptor.resize(16 * 16);
    tile_descriptor[16 * 2 + 2] = 3;
    tile_descriptor[16 * 2 + 3] = 3;
    tile_descriptor[16 * 2 + 5] = 3;
    tile_descriptor[16 * 8 + 2] = 3;
    tile_descriptor[16 * 4 + 8] = 8;
    tile_descriptor[16 * 4 + 9] = 9;
    tile_map.load(std::move(tile_descriptor));
    world->setTileMap(std::move(tile_map));

    world->add(ComponentGeometry{.x = 10, .y = 10, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
               ComponentSprite{TextureAtlas{box_tex, AtlasSizePx{64, 64}, TileSizePx{64, 64}}, 0.0f,
                               SpriteType::Static, 0u},
               ComponentCollider{40, 56, true});

    world->add(
        ComponentGeometry{.x = 200, .y = 100, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{TextureAtlas{box_tex, AtlasSizePx{64, 64}, TileSizePx{64, 64}}, 0.0f,
                        SpriteType::Static, 0u},
        ComponentCollider{40, 56, true});

    world->add(
        ComponentGeometry{.x = 300, .y = 100, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{TextureAtlas{box_tex, AtlasSizePx{64, 64}, TileSizePx{64, 64}}, 0.0f,
                        SpriteType::Static, 0u},
        ComponentCollider{40, 56, true});

    world->addPlayer(
        "Player One",
        ComponentGeometry{.x = 200, .y = 200, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{TextureAtlas{player_tex, AtlasSizePx{256, 64}, TileSizePx{64, 64}}, 0.0f,
                        SpriteType::Animated, 8u},
        ComponentCollider{40, 56});

    game.loadWorld(world);

    return game.start();
}