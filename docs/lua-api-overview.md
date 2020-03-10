# Lua API: Overview

## Table of contents

- [Lua API](#lua-api)
    - [Core API](#core-api)
    - [Mod API](#mod-api)
    - [Custom GUI](#custom-gui)
        - [Button](#button)
        - [Inventory](#inventory)
        - [Crafting table](#crafting-table)
        - [Image](#image)
- [C++ classes in Lua](#c-classes-in-lua)
    - [Registry](#registry)
    - [World](#world)
    - [Chunk](#chunk)
    - [BlockData](#blockdata)
    - [Player](#player)
    - [Inventory](#inventory-1)
    - [Recipe](#recipe)
    - [ItemStack](#itemstack)
    - [Item](#item)

## Lua API

### Core API

- `Registry *openminer:registry()`
- `openminer:add_listener(event_type, listener)`

### Mod API

Here is an example:

```lua
local mod = LuaMod.new("mymod")

mod:block {
	id = "myblock",
	name = "My Block",
	tiles = "myblock.png",
	hardness = 3,
	harvest_requirements = 1,
	on_tick = function(pos, player, chunk, world) end,
	on_block_activated = function(pos, player, world, client) end,
}

mod:item {
	id = "mymod:myitem",
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
	input = {id = "mymod:myblock", amount = 1},
	output = {id = "default:ingot_iron", amount = 1}
}
```

I'll describe all the possible options later.

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

### Registry

- `Recipe get_recipe(Inventory crafting_inventory)`

### World

- `u16 get_block(int x, int y, int z)`
- `u16 get_data(int x, int y, int z)`
- `void set_data(int x, int y, int z, u16 data)`
- `BlockData *get_block_data(int x, int y, int z)`

### Chunk

- `u16 get_block(int x, int y, int z)`
- `u16 get_data(int x, int y, int z)`
- `BlockData *get_block_data(int x, int y, int z)`

### BlockData

- `Inventory *inventory()`
- `u32 data()`

### Player

- `Inventory *inventory()`

### Inventory

- `void add_stack(string name, u16 amount)`
- `ItemStack get_stack(u16 x, u16 y)`
- `void set_stack(u16 x, u16 y, string name, u16 amount)`

### Recipe

- `string type()`
- `ItemStack result()`

### ItemStack

- `u16 amount()`
- `Item item()`

### Item

- `u16 id()`
- `string name()`
- `u16 burn_time()`

