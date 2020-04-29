# Lua API: Scroll bar

## Example

```lua
gui:scroll_bar {
	name = "scroll_bar",
	pos = {x = 175, y = 18},

	texture = mod:path() .. "/textures/gui/tabs.png",
	clip = {x = 232, y = 0, width = 12, height = 15},

	widget = "inv_creative_items",

	min_y = 0,
	max_y = 110 - 15,
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

### `max_y`

Maximum Y position for the bar.

Example:
```lua
max_y = 95
```

### `min_y`

Minimum Y position for the bar.

Example:
```lua
min_y = 0
```

### `name`

Name of the widget. **Mandatory field.**

Example:
```lua
name = "scroll_bar"
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
texture = mod:path() .. "/textures/gui/tabs.png"
```

See [this page](/lua-api-mod#path) for more details about `mod:path()`.

### `widget`

Name of the inventory widget which will be affected by the scroll bar.

Example:
```lua
widget = "inv_items"
```

