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

	blocks = {
		top = "default:sand",
		ground = "default:sand",
		deep = "default:stone",
		beach = "default:sand",
		liquid = "default:water",
		portal = "default:portal",
		portal_frame = "default:obsidian",
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
}
```

## Attributes

### `blocks`

Table containing the blocks used for the terrain.

Example:
```lua
blocks = {
	top = "default:grass",
	ground = "default:dirt",
	deep = "default:stone",
	beach = "default:sand",
	liquid = "default:water",
	portal = "default:portal",
	portal_frame = "default:obsidian",
}
```

#### `beach`

String ID of the block used around and under `liquid` block.

Example:
```lua
beach = "default:sand"
```

#### `deep`

String ID of the block used for the lower part of the terrain, below `ground` block.

Example:
```lua
deep = "default:stone"
```

#### `ground`

String ID of the block used for the upper part of the terrain, under `top` block.

Example:
```lua
ground = "default:dirt"
```

#### `liquid`

String ID of the block used to make lakes under `SEALEVEL` (see EngineConfig.hpp).

Example:
```lua
liquid = "default:water"
```

#### `portal`

String ID of the block used to generate portal center.

Example:
```lua
portal = "default:portal"
```

**Note:** This attribute is temporary and will get removed soon.

#### `portal_frame`

String ID of the block used to generate portal frame.

Example:
```lua
portal_frame = "default:obsidian"
```

**Note:** This attribute is temporary and will get removed soon.

#### `top`

String ID of the block used on the top of the terrain.

Example:
```lua
top = "default:grass"
```

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

