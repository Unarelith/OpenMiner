# Lua API: Inventory data

## Example

```lua
gui:inventory_data {
	name = "inv_data",

	width = 9,
	height = 1,

	items = {
		{"default:cobblestone", 10},
		{"default:stick", 64}
	},

	is_unlimited = true,
}
```

## Attributes

### `height`

Height of the inventory.

Example:
```lua
height = 1
```

### `is_unlimited`

Defines if the inventory is an infinite source of items or not.

Example:
```lua
is_unlimited = false -- this is the default value
```

### `items`

Table containing item stacks. Format of the stack table is `{id, amount}`.

Example:
```lua
items = {
	{"default:cobblestone", 10},
	{"default:stick", 64}
}
```

### `name`

Name of the widget. **Mandatory field.**

Example:
```lua
name = "inv_data"
```

### `width`

Width of the inventory.

Example:
```lua
width = 9
```

