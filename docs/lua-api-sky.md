# Lua API: Sky

## Example

```lua
mod:sky {
	id = "sky_overworld", -- mandatory

	color = {
		day = {50, 153, 204},
	},

	fog_color = {
		day = {50, 153, 204},
	},

	daylight_cycle = {
		speed = 1.0
	},

	objects = {
		sun = {
			texture = "texture-sun",
			size = 256,
		},

		moon = {
			texture = "texture-moon_phases",
			size = 256,
			phases = {
				count = 8,
				size = 32
			}
		},

		stars = {
			count = 1000,
			size = 4,
		}
	}
}
```

## Attributes

### `color`

Sky color.

Example:
```lua
color = {
	day = {154, 50, 33}
}
```

Attributes:

- `day`: sky color at midday

### `daylight_cycle`

Day/night cycle parameters.

Example:
```lua
daylight_cycle = {
	speed = 1.0
}
```

The example above is the minimal code required to add a day/night cycle to a sky.

Attributes:

- `speed`: speed of the cycle (default: `1.0`)

### `fog_color`

Fog color.

Example:
```lua
fog_color = {
	day = {154, 50, 33}
}
```

Attributes:

- `day`: gog color at midday

### `id`

ID of the sky. **Mandatory field.**

Example:
```lua
id = "sky_nether"
```

IDs are usually of the form `mod:sky` but the `mod:` prefix is prepended automatically so it's not needed.

### `objects`

#### `moon`

Moon attributes table.

Example:
```lua
moon = {
	texture = "texture-moon_phases"
	size = 256,
	phases = {
		count = 8,
		size = 32
	}
},
```

Attributes:

- `texture`: texture to use (without texture, the moon will use this color: (240, 240, 240))
- `size`: size of the moon (default: `256`)
- `phases`
    - `count`: amount of phases (default: `1`)
    - `size`: size of the phase texture (default: `32`)

#### `sun`

Sun attribute table.

Example:
```lua
sun = {
	texture = "texture-sun",
	size = 256,
},
```

Attributes:

- `texture`: texture to use (without texture, the sun will use this color: (255, 255, 0))
- `size`: size of the sun (default: `256`)

#### `stars`

Stars attribute table.

Example:
```lua
stars = {
	count = 1000,
	size = 4,
}
```

Attributes:

- `count`: size of the sun (default: `1000`)
- `size`: size of the sun (default: `4`)

