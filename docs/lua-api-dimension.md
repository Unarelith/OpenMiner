# Lua API: Dimension

## Example

```lua
mod:dimension {
	id = "overworld",   -- mandatory
	name = "Overworld", -- mandatory

	biomes = {"default:grassland", "default:desert"},

	sky = "default:sky_overworld",
}
```

## Attributes

### `biomes`

Table containing all the string IDs of the possible biomes for this dimension.

Example:
```lua
biomes = {"default:grassland", "default:desert"}
```

### `id`

ID of the dimension. **Mandatory field.**

Example:
```lua
id = "dimension_nether"
```

IDs are usually of the form `mod:dimension` but the `mod:` prefix is prepended automatically so it's not needed.

### `name`

Label of the dimension. **Mandatory field.**

Example:
```lua
name = "Nether"
```

This label is the name that will appear everywhere in the game.

### `sky`

String ID of the sky definition used for this dimension.

Example:
```lua
sky = "default:sky_nether"
```

