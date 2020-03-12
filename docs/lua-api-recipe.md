# Lua API: Recipe

## Example

### Crafting recipe

```lua
mod:crafting_recipe {
	result = {
		id = "default:wooden_axe",
		amount = 1
	},

	pattern = {
		"##",
		"#|",
		" |"
	},

	keys = {
		['#'] = "default:oak_planks",
		['|'] = "default:stick",
	}
}
```

### Smelting recipe

```lua
mod:smelting_recipe {
	input = {
		id = "default:iron_ore",
		amount = 1
	},

	output = {
		id = "default:iron_ingot",
		amount = 1
	}
}
```

## Attributes

### Crafting recipe

#### `result`

Item stack created with the craft.

Example
```lua
result = {
	id = "default:wooden_axe",
	amount = 1
}
```

Possible attributes:

- `id`: String ID of the item created.
- `amount`: Amount of items in the created stack

#### `pattern`

Pattern of the craft.

Example:
```lua
pattern = {
	"##",
	"#|",
	" |",
}
```

#### `keys`

Mapping of the characters used in `pattern` to actual item string IDs.

Example:
```lua
keys = {
	['#'] = "default:oak_planks",
	['|'] = "default:stick",
}
```

### Smelting recipe

#### `input`

Item stack used as input for the recipe.

Example:
```lua
input = {
	id = "default:cobblestone",
	amount = 1
}
```

Possible attributes:

- `id`: String ID of the item created.
- `amount`: Amount of items in the created stack

#### `output`

Item stack created with the recipe.

Example:
```lua
output = {
	id = "default:stone",
	amount = 1
}
```

Possible attributes:

- `id`: String ID of the item created.
- `amount`: Amount of items in the created stack

