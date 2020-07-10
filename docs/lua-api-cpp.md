# Lua API: C++ classes in Lua

**Note:** This page is currently really basic but it will be extended later.

## Block

- `u16 id()`
- `string string_id()`
- `string mod_name()`
- `BlockParam param()`
- `Block get_state()`

## BlockData

- `Inventory *inventory`
- `BlockMetadata *meta`
- `bool use_alt_tiles`

## BlockMetadata

- `string get_string(string attribute)`
- `void set_string(string attribute, string value)`
- `int get_int(string attribute)`
- `void set_int(string attribute, int value)`
- `bool get_bool(string attribute)`
- `void set_bool(string attribute, bool value)`

## Chunk

- `u16 get_block(int x, int y, int z)`
- `void set_block(int x, int y, int z, u16 block)`
- `u16 get_data(int x, int y, int z)`
- `void set_data(int x, int y, int z, u16 data)`
- `BlockData *add_block_data(int x, int y, int z, int inventoryWidth, int inventoryHeight)`
- `BlockData *get_block_data(int x, int y, int z)`

## ClientInfo

- `u16 id()`

## Dimension

- `u16 id()`

## EntityWrapper

- `entt::entity id()`
- `PositionComponent *position()`
- `NetworkComponent *network()`
- `ItemStack *item_stack()`

## Inventory

- `void add_stack(string name, u16 amount)`
- `ItemStack get_stack(u16 x, u16 y)`
- `void set_stack(u16 x, u16 y, string name, u16 amount)`

## Item

- `u16 id()`
- `string name()`
- `bool has_group(string groupName)`
- `u16 get_group_value(string groupName)`

## ItemStack

- `u16 amount()`
- `Item item()`

## ivec3

- `int x`
- `int y`
- `int z`

## Player

- `string name()`
- `double x()`
- `double y()`
- `double z()`
- `void set_position(double x, double y, double z)`
- `u16 dimension()`
- `void set_dimension(u16 dimension)`
- `u16 client_id()`
- `Inventory *inventory()`

## Recipe

- `string type()`
- `ItemStack result()`

## Registry

- `Block get_block(u16 id)`
- `Item get_item(u16 id)`
- `Sky get_sky(u16 id)`
- `Tree get_tree(u16 id)`
- `Biome get_biome(u16 id)`
- `Recipe get_recipe(Inventory crafting_inventory)`
- `Block get_block_from_string(string id)`
- `Item get_item_from_string(string id)`
- `Sky get_sky_from_string(string id)`
- `Tree get_tree_from_string(string id)`
- `Biome get_biome_from_string(string id)`
- `Block[] blocks()`
- `Item[] items()`
- `Tree[] trees()`
- `Biome[] biomes()`
- `Dimension[] dimensions()`

## ServerCommandHandler

- `void send_player_change_dimension(u16 clientID, int x, int y, int z, u16 dimension, ClientInfo client)`
- `void send_chat_message(u16 senderID, string message, ClientInfo client)`

## ServerModLoader

- `LuaMod *register_mod(string mod_id)` (see [Lua Mod API](lua-api-mod.md#example))

## ServerPlayer

- `const ClientInfo &client()`
- `ItemStack held_item_stack()`
- `bool is_online()`

## ServerWorld

- `u16 dimension()`

## World

- `u16 get_block(int x, int y, int z)`
- `void set_block(int x, int y, int z, u16 block)`
- `u16 get_data(int x, int y, int z)`
- `void set_data(int x, int y, int z, u16 data)`
- `BlockData *add_block_data(int x, int y, int z, int inventoryWidth, int inventoryHeight)`
- `BlockData *get_block_data(int x, int y, int z)`
- `const Block &get_block_def(int x, int y, int z)`
- `void set_block_from_str(int x, int y, int z, string block_id)`

