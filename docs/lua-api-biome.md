# Lua API: Biome

## Example

```lua
mod:biome {
	id = "desert",   -- mandatory
	name = "Desert", -- mandatory

	params = {
		temperature = -0.3,
		precipitation = -0.7
	},

	trees = {
		{
			type = "default:tree_cactus",
			probability = 0.001,
		}
	},

	flora = {
		{
			block = "default:deadbush",
			spawns_on = "default:sand",
			probability = 0.005,
		}
	},

	top_block = "default:sand",
	ground_block = "default:sand",
	deep_block = "default:stone",
	beach_block = "default:sand",
	liquid_block = "default:water",
	portal_block = "default:portal",
	portal_frame_block = "default:obsidian",
}
```

## Attributes

### `flora`

Table that defines flora placement.

Example:
```lua
flora = {
	{
		block = "default:tallgrass",
		spawns_on = "default:grass",
		probability = 0.25
	},
	{
		block = "default:dandelion",
		spawns_on = "default:grass",
		probability = 0.025
	}
}
```

Possible attributes:
- `block`: Block used as flora
- `spawn_on`: What the block below must be
- `probability`: Probability to spawn `block`

### `id`

ID of the biome. **Mandatory field.**

Example:
```lua
id = "biome_desert"
```

IDs are usually of the form `mod:biome` but the `mod:` prefix is prepended automatically so it's not needed.

### `name`

Label of the biome. **Mandatory field.**

Example:
```lua
name = "Desert"
```

This label is the name that will appear everywhere in the game.

### `ores`

Table that defines ore placement.

Example:
```lua
ores = {
	{
		block = "default:iron_ore",
		probability = 0.0003,
		size = 8
	}
}
```

Possible attributes:
- `block`: Ore block to be used
- `probability`: Probability to spawn an ore vein with that block
- `size`: Ore vein size

### `params`

Parameters used to generate the biome.

Example:
```lua
params = {
	temperature = -0.3,
	precipitation = -0.7
}
```

Current valid parameters are:

- `temperature`
- `precipitation`

### `trees`

Table that defines tree placement.

Example:
```lua
trees = {
	{
		type = "default:oak",
		probability = 0.01
	},
	{
		type = "default:cactus",
		probability = 0.001
	}
}
```

Possible attributes:
- `type`: String ID of the tree definition
- `probability`: Probability to spawn this tree

### Blocks

#### `top_block`

String ID of the block used on the top of the terrain.

Example:
```lua
top_block = "default:grass"
```

#### `ground_block`

String ID of the block used for the upper part of the terrain, under `top_block`.

Example:
```lua
ground_block = "default:dirt"
```

#### `deep_block`

String ID of the block used for the lower part of the terrain, below `ground_block`.

Example:
```lua
deep_block = "default:stone"
```

#### `beach_block`

String ID of the block used around and under `liquid_block`.

Example:
```lua
beach_block = "default:sand"
```

#### `liquid_block`

String ID of the block used to make lakes under `SEALEVEL` (see EngineConfig.hpp).

Example:
```lua
liquid_block = "default:water"
```

#### `portal_block`

String ID of the block used to generate portal center.

Example:
```lua
portal_block = "default:portal"
```

**Note:** This attribute is temporary and will get removed soon.

#### `portal_frame_block`

String ID of the block used to generate portal frame.

Example:
```lua
portal_frame_block = "default:obsidian"
```

**Note:** This attribute is temporary and will get removed soon.

