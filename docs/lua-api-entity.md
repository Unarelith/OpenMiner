# Lua API: Entity

## Example

```lua
mod:entity {
	id = "item_drop",

	properties = {
		visual = {
			type = "inventorycube",
			size = 0.25,
			origin = {0.125, 0.125, 0.125},
		},

		is_rotatable = true,

		animation = {
			{
				type = "rotation",
				axis = {0, 0, 1},
				angle = 0.5
			},
			{
				type = "translation",
				delta = {0, 0, -0.0005},
				min = -0.2,
				max = 0,
				loop = true
			}
		},

		hitbox = {0, 0, 0, 0.25, 0.25, 0.25},
	},

	on_collision = function(entity, player, server)
		mods["default"]:give_item_stack(player, entity:item_stack());
		mods["default"]:despawn_entity(entity)
	end,
}
```

## Attributes

### `id`

ID of the entity. **Mandatory field.**

Example:
```lua
id = "item_drop"
```

### `properties`

Properties table.

#### `animation`

Animation table used to define client-side animation.

Example:
```lua
animation = {
	{
		type = "rotation",
		axis = {0, 0, 1},
		angle = 0.5
	},
	{
		type = "translation",
		delta = {0, 0, -0.0005},
		min = -0.2,
		max = 0,
		loop = true
	}
}
```

**NB:** You need to set `is_rotatable` to `true` if using `rotation` animation.

#### `hitbox`

Hitbox of the entity.

Defined using an array with respectively `{x, y, z, width, depth, height}`.

Example:
```lua
hitbox = {0, 0, 0, 0.25, 0.25, 0.25}
```

#### `is_rotatable`

Defines if the entity can be rotated or not. Default: `false`.

Example:
```lua
is_rotatable = true
```

#### `visual`

Visual definition.

Example:
```lua
visual = {
    type = "inventorycube",
    size = 0.25,
    origin = {0.125, 0.125, 0.125},
    block_id = "default:cobblestone"
}
```

**NB:** Currently, `inventorycube` is the only visual type available.

## Callbacks

### `on_collision`

Called when a player is colliding with the entity.

Example:
```lua
on_collision = function(entity, player, server)
    mods["default"]:give_item_stack(player, entity:item_stack());
    mods["default"]:despawn_entity(entity)
end
```

**NB:** `mods` is a global array here. If you use `mod` global instead, it won't work if another mod overwrites it, that's why using an array is better.

## Entity spawn parameters

### `dimension`

Dimension ID of the entity. **Mandatory field.**

Example:
```lua
dimension = 1
```

### `item_stack`

Item stack component for the entity.

Example:
```lua
item_stack = {"default:cobblestone", 64} -- {string_id, amount}
```

### `position`

Position of the entity. **Mandatory field.**

Example:
```lua
position = {5, 2, 3}
```

