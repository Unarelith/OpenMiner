# Lua API: Overview

## Lua API

Note: The Lua API is currently server-side only.

### Core API

#### `openminer:registry()`

This function returns the `Registry` instance of the server.

#### `openminer:add_listener(event_type, listener)`

Adds a listener to a specific type of event.

Example:
```lua
openminer:add_listener(EventType.OnBlockPlaced, function(pos, player, world, client, server)
	server:send_chat_message(0, "Block placed at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client);
end)

openminer:add_listener(EventType.OnBlockActivated, function(pos, block, player, world, client, server)
	if block:string_id() == "default:portal" then
		server:send_chat_message(0, "Swoosh! Changing dimension...", client);
	end
end)
```

Possible events:

- `OnBlockPlaced`: `funcion(pos, player, world, client, server)`
- `OnBlockActivated`: `function(pos, block, player, world, client, server)`

### Mod API

Here is an example:

```lua
local mod = LuaMod.new("mymod")

mod:block {
	id = "myblock",
	name = "My Block",
	tiles = "myblock.png",
}

mod:item {
	id = "myitem",
	name = "My Item",
	tiles = "myitem.png",
}

mod:crafting_recipe {
	result = {
		id = "mymod:myblock",
		amount = 1
	},

	pattern = {
		"###",
		"# #",
		"###"
	},

	keys = {["#"] = "default:cobblestone"}
}

mod:smelting_recipe {
	input = {id = "mymod:myitem", amount = 1},
	output = {id = "mymod:myblock", amount = 1}
}
```

### Custom GUI

You can create a new GUI with:
```lua
local gui = LuaGUI.new()
```

To show the custom GUI use:
```lua
gui:show(client)
```

#### Button

```lua
gui:button {
	name = "btn_test",
	pos = {x = 0, y = 0},

	text = "Test button",
	on_click = function(self)
		print("Test button pressed")
	end,
}
```

#### Inventory

```lua
gui:inventory {
	name = "inv_main",
	pos = {x = gui_pos.x + 7, y = gui_pos.y + 83},

	player = "player",
	inventory = "main",
	size = {x = 9, y = 3},
	offset = 9,
	count = 9 * 3,
}
```

#### Crafting table

```lua
gui:crafting {
	name = "inv_crafting",
	pos = {x = gui_pos.x, y = gui_pos.y},

	block = {x = pos.x, y = pos.y, z = pos.z},

	offset = 0,
}
```

#### Image

```lua
gui:image {
	name = "img_background",
	pos = gui_pos,

	texture = "texture-workbench",
	clip = {x = 0, y = 0, width = 176, height = 166},
}
```

## C++ classes in Lua

### Block

- `u16 id()`
- `u16 data()`
- `string string_id()`
- `string label()`
- `stirng mod_name()`
- `bool is_opaque()`

### BlockData

- `Inventory *inventory`
- `BlockMetadata *meta`
- `bool use_alt_tiles`

### BlockMetadata

- `string get_string(string attribute)`
- `void set_string(string attribute, string value)`
- `int get_int(string attribute)`
- `void set_int(string attribute, int value)`

### Chunk

- `u16 get_block(int x, int y, int z)`
- `void set_block(int x, int y, int z, u16 block)`
- `u16 get_data(int x, int y, int z)`
- `void set_data(int x, int y, int z, u16 data)`
- `BlockData *add_block_data(int x, int y, int z, int inventoryWidth, int inventoryHeight)`
- `BlockData *get_block_data(int x, int y, int z)`

### ClientInfo

- `u16 id()`

### Inventory

- `void add_stack(string name, u16 amount)`
- `ItemStack get_stack(u16 x, u16 y)`
- `void set_stack(u16 x, u16 y, string name, u16 amount)`

### Item

- `u16 id()`
- `string name()`
- `u16 burn_time()`
- `bool is_fuel()`

### ItemStack

- `u16 amount()`
- `Item item()`

### ivec3

- `int x`
- `int y`
- `int z`

### Player

- `Inventory *inventory()`
- `double x()`
- `double y()`
- `double z()`
- `void set_position(double x, double y, double z)`
- `u16 dimension()`
- `void set_dimension(u16 dimension)`
- `u16 client_id()`

### Recipe

- `string type()`
- `ItemStack result()`

### Registry

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

### ServerCommandHandler

- `void send_player_change_dimension(u16 clientID, int x, int y, int z, u16 dimension, ClientInfo client)`
- `void send_chat_message(u16 senderID, string message, ClientInfo client)`

### World

- `u16 get_block(int x, int y, int z)`
- `void set_block(int x, int y, int z, u16 block)`
- `u16 get_data(int x, int y, int z)`
- `void set_data(int x, int y, int z, u16 data)`
- `BlockData *add_block_data(int x, int y, int z, int inventoryWidth, int inventoryHeight)`
- `BlockData *get_block_data(int x, int y, int z)`

