#include "WillEngine/Game.hpp"
#include "WillEngine/systems/BaseStateMashine.hpp"
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
        ComponentCollider{.hitbox_w=40, .hitbox_h=56, .pushable=true});

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


class StateMachinePlayer : public BaseStateMashine {

public:
    explicit StateMachinePlayer(entt::entity id) : BaseStateMashine(id) {}

    auto tick() -> void override {
        if (getRegistry() == nullptr) {
            return;
        }

        auto &[input_keyboard] = getRegistry()->get<ComponentInput>(getEntittyId());
        auto &sprite = getRegistry()->get<ComponentSprite>(getEntittyId());

        auto in_x = input_keyboard.x;
        auto in_y = input_keyboard.y;
        if (in_x != 0 || in_y != 0) {
            sprite.type = SpriteType::Animated;
        } else {
            sprite.type = SpriteType::Static;
            sprite.frame_float = 0.0;
        }
    }
};


template <typename TileType>
auto build_player(Game &game, World<TileType> &world, const std::string &texture_name) {

    auto [entity, id] = world.addPlayer(
        "Player One",
        ComponentGeometry{.x = 400, .y = 600, .z = 0, .size_x = 64, .size_y = 64, .size_z = 0},
        ComponentSprite{.atlas =
                            TextureAtlas{texture_name, AtlasSizePx{256, 64}, TileSizePx{64, 64}},
                        .frame_float = 0.0f,
                        .type = SpriteType::Animated,
                        .fps = 8u},
        ComponentCollider{.hitbox_w = 40, .hitbox_h = 56});

    auto sm = std::make_unique<StateMachinePlayer>(entity);
    game.addStateMachine(std::move(sm));
    return entity;
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
    build_player(game, *world, player_tex);

    game.loadWorld(world);

    // Start
    return game.start();
}
