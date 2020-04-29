# Lua API: Progress bar

## Example

```lua
gui:progress_bar {
	name = "bar_process",
	pos = {x = 80, y = 35},

	type = "item_process",

	block = {x = pos.x, y = pos.y, z = pos.z},

	meta = "item_progress",
	max_value = 200,

	texture = mod:path() .. "/textures/gui/furnace.png",
	clip = {x = 176, y = 14, width = 24, height = 17},
}
```

## Attributes

### `block`

Specify the block to use for metadata.

```lua
block = {
	x = 0,
	y = 0,
	z = 0
}
```

### `clip`

Clip rect of the texture.

Example:
```lua
clip = {
	x = 0,
	y = 0,
	width = 176,
	height = 166
}
```

### `max_meta`

Name of the metadata value storing the max value of the bar in the `block`.

Example:
```lua
max_meta = "current_burn_time"
```

### `max_value`

Max value of the progress bar. Used with `type = "item_process"`.

Example:
```lua
max_value = 200,
```

### `meta`

Name of the metadata value storing the progress of the bar in the `block`.

Example:
```lua
meta = "ticks_remaining"
```

### `name`

Name of the widget. **Mandatory field.**

Example:
```lua
name = "bar_burn"
```

### `pos`

Position of the widget. **Mandatory field.**

Example:
```lua
pos = {
	x = 0,
	y = 0
}
```

### `texture`

Full path of the texture. Relative to the repo root.

Example:
```lua
texture = mod:path() .. "/textures/gui/furnace.png"
```

See [this page](/lua-api-mod#path) for more details about `mod:path()`.

### `type`

Type of the progress bar.

Example:
```lua
type = "item_process"
```

Possible values:

- `item_process`: Goes from left to right depending on `meta` and `max_value`.
- `burn_process`: Goes from down to up depending on `meta` and `max_meta`.

