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

Possible values:

- `day`: Sky color at midday

### `fog_color`

Fog color.

Example:
```lua
fog_color = {
	day = {154, 50, 33}
}
```

Possible values:

- `day`: Fog color at midday

### `id`

ID of the sky. **Mandatory field.**

Example:
```lua
id = "sky_nether"
```

IDs are usually of the form `mod:sky` but the `mod:` prefix is prepended automatically so it's not needed.

