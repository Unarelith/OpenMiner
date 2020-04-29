# Lua API: Image

## Example

```lua
gui:image {
	name = "img_background", -- mandatory
	pos = {x = 0, y = 0},    -- mandatory

	texture = mod:path() .. "/textures/gui/workbench.png",
	clip = {x = 0, y = 0, width = 176, height = 166},
}
```

## Attributes

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

### `name`

Name of the widget. **Mandatory field.**

Example:
```lua
name = "img_background"
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
texture = mod:path() .. "/textures/gui/workbench.png"
```

See [this page](/lua-api-mod#path) for more details about `mod:path()`.

