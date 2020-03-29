# Lua API: Mod API Overview

## Introduction

You can create a mod using this function:
```
LuaMod.new("mod_name")
```

Mod name should match this regex: `[a-zA-Z0-9_]+`

Everytime your define an `id` for a mod element, this name will be prepended to it.
For example, `myitem` will become `mymod:myitem`.

The name can't be `group` because this namespace is already used by the engine.

## Example

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

## Functions

### `block`

Defines a block from a table, see [this page](lua-api-block.md) for more information.

### `biome`

Defines a biome from a table, see [this page](lua-api-biome.md) for more information.

### `dimension`

Defines a dimension from a table, see [this page](lua-api-dimension.md) for more information.

### `item`

Defines an item from a table, see [this page](lua-api-item.md) for more information.

### `recipe`

Defines a recipe from a table, see [this page](lua-api-recipe.md) for more information.

### `sky`

Defines a sky type from a table, see [this page](lua-api-sky.md) for more information.

### `tree`

Defines a tree type from a table, see [this page](lua-api-tree.md) for more information.

