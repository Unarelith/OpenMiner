# Lua API: Inventory Widget

## Example

```lua
gui:inventory {
	name = "inv_main",     -- mandatory field
	pos = {x = 7, y = 83}, -- mandatory field

	inventory = {
		source = "player",
		player = "player",
		inventory_name = "main",
		offset = 9,
		count = 9 * 3,
	},

	size = {x = 9, y = 3},

	shift_destination = "inv_hotbar,inv_main",
}
```

## Attributes

### `filter`

Use a group as a filter for this inventory widget.

Example:
```
filter = "group:om_fuel"
```

### `inventory`

Details of the inventory to use.

Example:
```lua
inventory = {
	source = "block",
	block = {x = 0, y = 10, z = 20},
	offset = 1,
	count = 1,
}
-- or
inventory = {
	source = "player",
	player = "player",
	inventory_name = "main",
	offset = 9,
	count = 9 * 3,
}
-- or
inventory = {
	source = "temp",
	inventory_name = "inv_data",
	count = 9 * 5,
}
```

#### `block`

Specify the block to use with `block` inventory source.

```lua
block = {
	x = 0,
	y = 0,
	z = 0
}
```

#### `count`

Size of the subset of the source inventory.

Example:
```lua
count = 27
```

#### `inventory_name`

Specify the inventory to use. Used with `player` and `temp` inventory sources.

Example:
```lua
inventory_name = "main"
```

#### `offset`

Offset of the subset of the target inventory.

Example:
```lua
offset = 0 -- this is the default value
```

#### `player`

**Note:** Placeholder. Currently unused.

Example:
```lua
player = "player"
```

#### `source`

Source of the inventory displayed.

Example:
```lua
source = "player"
```

Possible values:

- `player`: Set a specific player as the source
- `block`: Set a specific block as the source
- `temp`: Set a temporary inventory as the source

### `is_read_only`

Defines if the inventory is read-only or not.

Example:
```lua
is_read_only = false -- this is the default value
```

### `name`

Name of the widget. **Mandatory field.**

Example:
```lua
name = "inv_main"
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

