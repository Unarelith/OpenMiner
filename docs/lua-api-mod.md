# Lua API: Mod API Overview

## Intrduction

Mods are folders and they should contain at least two files:

- `init.lua`: your code
- `config.lua`: mod config (name, depedencies)

They are located in `mods` folder.

## Mod config

Here is an example of configuration:
```lua
mod_config = {
	id = "creative_inventory",

	dependencies = {"*default"}
}
```

The `*` before `default` means it's an optional dependency.

## How to use this API

You can create a mod using this function:
```
openminer.mod_loader:register_mod("mod_name")
```

Mod name should match this regex: `^[a-zA-Z0-9_]+$`

Everytime you define an `id` for a mod element, this name will be prepended to it.
For example, `myitem` will become `mymod:myitem`.

The name can't be `group` because this namespace is already used by the engine.

## Example

```lua
local mod = openminer.mod_loader:register_mod("mymod")

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

## Functions

### `id`

Returns the string ID of the mod.

### `path`

Returns the path of the mod, relative to current working directory.

**Note:** In callbacks like `on_block_activated`, the `mod` object may be invalid.
You'll need to create a local variable at the beginning of the file:
```lua
local modpath = mod:path()
```

## Utility functions

### `despawn_entity`

Despawn an entity.

Example:
```lua
mod:despawn_entity(entity)
```

**NB:** `entity` is an `EntityWrapper` instance here. Currently, only `on_collision` entity callback can use it.

### `give_item_stack`

Give an item stack to a player.

Example:
```lua
mod:give_item_stack(player, item_stack)
```

### `spawn_entity`

Spawn an entity.

Example:
```lua
mod:spawn_entity("default:item_drop", {
	position = {pos.x + 0.5, pos.y + 0.5, pos.z + 0.5},
	dimension = world:dimension():id(),

	item_stack = {block:get_item_drop():item():string_id(), block:get_item_drop():amount()}
})
```

See [entity page](lua-api-entity.md#entity-spawn-parameters) for more information about the available fields in the table.

## Registration functions

### `config`

Defines a config option.

Example:
```lua
mod:option("use_item_drops", false);
```

See [this page](lua-api-core.md#openminerget_configname) for more details.

### Other functions

- [block](lua-api-block.md)
- [biome](lua-api-biome.md)
- [dimension](lua-api-dimension.md)
- [entity](lua-api-entity.md)
- [item](lua-api-item.md)
- [key](lua-api-key.md)
- [recipe](lua-api-recipe.md)
- [sky](lua-api-sky.md)
- [tree](lua-api-tree.md)

