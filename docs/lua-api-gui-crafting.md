# Lua API: Crafting Widget

## Example

```lua
gui:crafting {
	name = "inv_crafting",
	pos = {x = 29, y = 16},
	result_pos = {x = 123, y = 34},

	inventory = "block",
	block = {x = pos.x, y = pos.y, z = pos.z},

	shift_destination = "inv_main,inv_hotbar",
}
```

## Attributes

### `block`

Specify the block to use with `block` inventory source.

```lua
block = {
	x = 0,
	y = 0,
	z = 0
}
```

### `inventory`

Source of the inventory displayed.

Example:
```lua
inventory = "block"
```

Possible values:

- `block`: Set a specific block as the source
- `temp`: Set a temporary inventory as the source

### `name`

Name of the widget. **Mandatory field.**

Example:
```lua
name = "inv_crafting"
```

### `offset`

Offset of the subset of the source inventory. Only used with `inventory = "block"`.

Example:
```lua
offset = 0
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

### `result_pos`

Position of the result slot of the crafting table.

Example:
```lua
result_pos = {
	x = 0,
	y = 0
}
```

### `shift_destination`

Names of the inventories used for the Shift+Click action, separated by `,`

Example:
```lua
shift_destination = "inv_hotbar,inv_main"
```

### `size`

Size of the crafting table.

Example:
```lua
size = 3, -- default value, 3x3 crafting table
```

