# Lua API: Inventory Widget

## Example

```lua
gui:inventory {
	name = "inv_main",     -- mandatory field
	pos = {x = 7, y = 83}, -- mandatory field

	inventory = "player",
	player = "player",
	inventory_name = "main",
	size = {x = 9, y = 3},
	offset = 9,
	count = 9 * 3,

	shift_destination = "inv_hotbar,inv_main",
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

### `count`

Size of the subset of the source inventory.

Example:
```lua
count = 27
```

### `inventory`

Source of the inventory displayed.

Example:
```lua
inventory = "player"
```

Possible values:

- `player`: Set a specific player as the source
- `block`: Set a specific block as the source
- `temp`: Set a temporary inventory as the source

### `inventory_name`

Specify the inventory to use. Used with `player` and `temp` inventory sources.

Example:
```lua
inventory_name = "main"
```

### `name`

Name of the widget. **Mandatory field.**

Example:
```lua
name = "inv_main"
```

### `offset`

Offset of the subset of the target inventory.

Example:
```lua
offset = 0
```

### `player`

**Note:** Placeholder. Currently unused.

Example:
```lua
player = "player"
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

### `shift_destination`

Names of the inventories used for the Shift+Click action, separated by `,`

Example:
```lua
shift_destination = "inv_hotbar,inv_main"
```

### `size`

Size of the inventory (in slots).

Example:
```lua
size = {
	x = 9,
	y = 3
}
```

