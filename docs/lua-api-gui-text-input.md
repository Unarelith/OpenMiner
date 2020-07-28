# Lua API: Text Input

**Note:** This only purpose of this atm is to filter items in an inventory widget. More uses will come later.

## Example

```lua
gui:text_input {
	name = "inv_filter",
	pos = {x = 82, y = 6},

	width = 86,
	height = 8,

	placeholder = "Search...",
	placeholder_color = {192, 192, 192},

	inventory = "inv_creative_items"
}
```

## Attributes

### `height`

Height of the widget. **Mandatory field.**

Example:
```lua
height = 100
```

### `inventory`

Name of the inventory widget to filter.

Example:
```lua
inventory = "inv_blocks"
```

### `name`

Name of the widget. **Mandatory field.**

Example:
```lua
name = "inv_main"
```

### `placeholder`

Placeholder text displayed when the box is empty.

Example:
```lua
placeholder = "Search..."
```

### `placeholder_color`

Placeholder text color.

Example:
```lua
placeholder_color = {192, 192, 192}
```

**Note:** Alpha is optional, defaults to max value (`255`).

### `pos`

Position of the widget. **Mandatory field.**

Example:
```lua
pos = {
	x = 0,
	y = 0
}
```

### `width`

Width of the widget. **Mandatory field.**

Example:
```lua
width = 100
```

