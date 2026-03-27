#include "WillEngine/Game.hpp"
#include "WillEngine/world/entity_components/ComponentCollider.hpp"
#include "WillEngine/world/entity_components/ComponentSprite.hpp"


using namespace will_engine;

template <typename TileType>
void build_tile_map(World<TileType> &world, const std::string &texture_name) {

    auto tile_map =
        TileMap<int>{ArraySize2D{16, 16}, TileSizePx{64, 64}, 0,
                     TextureAtlas{texture_name, AtlasSizePx{256, 256}, TileSizePx{64, 64}}};
    std::vector<int> tile_descriptor = {};
    tile_descriptor.resize(16 * 16);
    tile_descriptor[16 * 2 + 2] = 3;
    tile_descriptor[16 * 2 + 3] = 3;
    tile_descriptor[16 * 2 + 5] = 3;
    tile_descriptor[16 * 8 + 2] = 3;
    tile_descriptor[16 * 4 + 8] = 8;
    tile_descriptor[16 * 4 + 9] = 9;
    tile_map.load(std::move(tile_descriptor));
    world.setTileMap(std::move(tile_map));
}

template <typename TileType>
void build_boxes(World<TileType> &world, const std::string &texture_name) {

    world.add(
        ComponentGeometry{.x = 210, .y = 410, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas =
                            TextureAtlas{texture_name, AtlasSizePx{64, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Static,
                        .fps = 0u},
        ComponentCollider{40, 56, true});

    world.add(
        ComponentGeometry{.x = 400, .y = 500, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas =
                            TextureAtlas{texture_name, AtlasSizePx{64, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Static,
                        .fps = 0u},
        ComponentCollider{.hitbox_w = 40, .hitbox_h = 56, .pushable = true});

    world.add(
        ComponentGeometry{.x = 500, .y = 400, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas =
                            TextureAtlas{texture_name, AtlasSizePx{64, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Static,
                        .fps = 0u},
        ComponentCollider{.hitbox_w = 40, .hitbox_h = 56, .pushable = true});
}


template <typename TileType>
void build_player(World<TileType> &world, const std::string &texture_name) {

    world.addPlayer(
        "Player One",
        ComponentGeometry{.x = 400, .y = 600, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas =
                            TextureAtlas{texture_name, AtlasSizePx{256, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Animated,
                        .fps = 8u},
        ComponentCollider{.hitbox_w = 40, .hitbox_h = 56});
}


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
    build_player(*world, player_tex);
    game.loadWorld(world);

    // Start
    return game.start();
}