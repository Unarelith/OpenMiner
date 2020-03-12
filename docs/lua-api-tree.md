# Lua API: Tree

## Example

```lua
mod:tree {
	id = "oak",

	log_block = "default:oak_wood",
	leaves_block = "default:oak_leaves",

	trunk_height = {
		min = 3,
		max = 6
	},
}
```

## Attributes

### `id`

ID of the tree. **Mandatory field.**

Example:
```lua
id = "tree_oak"
```

IDs are usually of the form `mod:tree` but the `mod:` prefix is prepended automatically so it's not needed.

### `log_block`

String ID of the block used as a log.

Example:
```lua
log_block = "default:oak_wood"
```

### `leaves_block`

String ID of the block used as leaves if `has_leaves` is set to `true`.

Example:
```lua
leaves_block = "default:oak_leaves"
```

### `has_leaves`

Defines if the tree has leaves or not.

Example:
```lua
has_leaves = true -- this is the default value
```

### `trunk_height`

Minimum and maximum trunk height.

Example:
```lua
trunk_height = {
	min = 3,
	max = 6
}
```

