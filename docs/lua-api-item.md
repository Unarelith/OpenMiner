# Lua API: Item

## Example

```lua
mod:item {
	id = "myitem",
	name = "My Item",
	tiles = "myitem.png",
}
```

## Attributes

### `id`

ID of the item. **Mandatory field.**

Example:
```lua
id = "stick"
```

IDs are usually of the form `mod:item` but the `mod:` prefix is prepended automatically so it's not needed.

### `name`

Label of the item. **Mandatory field.**

Example:
```lua
name = "Stick"
```

This label is the name that will appear everywhere in the game.

### `tiles`

Texture of the item.

Example:
``` lua
tiles = "myblock.png"
```

The textures will be loaded from `mods/<your-mod>/textures/items`

**Note:** Currently, you can only use textures of the exact same size (16x16, 32x32) than the other block/item textures in the game.

### `harvest_capability`

For a tool, set which blocks are easier to mine.

**Note:** This attribute would need more doc but it'll probably get removed soon.

### `mining_speed`

For a tool, speed at which it mines the block.

Example:
```lua
mining_speed = 1 -- this is the default value
```

### `is_fuel`

Defines if the item is valid furnace fuel or not.

Example:
```lua
is_fuel = false -- this is the default value
```

### `burn_time`

Burn time of a fuel item.

Example:
```lua
burn_time = 200 -- example value for a coal item, default is 0
```

