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

### `player_physics`

Table containing the physics properties of the dimension

Example:
```lua
player_physics = {
	gravity = 1.0,
	jump_antigravity = 0.3,
}
```

#### `air_speed_mod`

The movement speed of players in this dimension is multiplied by this amount while in the air.

Example:
```lua
air_speed_mod = 0.75
```

Default value is `0.75`.

#### `fly_speed`

Fly speed in this dimension.

Example:
```lua
fly_speed = 0.75
```

Default value is `0.75`.

#### `glide_gravity`

Fall speed while holding Jump key in this dimension.

Example:
```lua
glide_gravity = 0.05
```

Default value is `0.05`.

#### `gravity`

Gravity of the dimension.

Example:
```lua
gravity = 0.5
```

Default value is `1`.

#### `horizontal_sprint_mod`

Horizontal sprint speed multiplier.

Example:
```lua
horizontal_sprint_mod = 1.5
```

Default value is `1.5`.

#### `is_sneak_always_mod`

Whether sneaking applies the horizontal mod while airborne.

Example:
```lua
is_sneak_always_mod = false
```

Default value is `false`.

#### `jump_antigravity`

Gravity is decreased by this amount while holding the jump key prior to the peak of the jump.

Example:
```lua
jump_antigravity = 0.04
```

Default value is `0.04`.

#### `jump_speed`

Jump speed of the dimension.

Example:
```lua
jump_speed = 0.3
```

Default value is `0.3`.

#### `move_speed`

Movement speed in this dimension.

Example:
```lua
move_speed = 0.04
```

Default value is `0.04`.

#### `sneak_horizontal_mod`

Horizontal movement speed is multiplied by this amount while sneaking.

Example:
```lua
sneak_horizontal_mod = 0.1
```

Default value is `0.1`.

#### `sneak_vertical_speed`

Downwards momentum of this amount is applied while sneaking.

Example:
```lua
sneak_vertical_speed = 0.1
```

Default value is `0.1`.

#### `vertical_sprint_mod`

Vertical sprint speed multiplier.

Example:
```lua
vertical_sprint_mod = 1.5
```

Default value is `1.5`.

### `sky`

String ID of the sky definition used for this dimension.

Example:
```lua
sky = "default:sky_nether"
```

