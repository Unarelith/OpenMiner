# Lua API: Block

## Example

```lua
mod:block {
	id = "myblock",    -- mandatory
	name = "My Block", -- mandatory
	tiles = "myblock.png",
}
```

## Attributes

### `bounding_box`

Bounding box of the box.

Example:
```lua
bounding_box = {0, 0, 0, 1, 1, 1} -- this is default value
```

Format is `{x, y, z, width, depth, height}`. Note that OpenMiner uses Z axis as up.

The bounding box determines collisions with the block, as well as the selection box that will be displayed when looking at the block.

Note: This is what affects the temporary `boundingbox` draw type.

### `color_multiplier`

Color multiplier of the grayscale parts of the block texture.

Example:
```lua
color_multiplier = {1, 1, 1, 1} -- full white, this is the default value
```

Only the pure gray parts (red == green == blue) will be affected by this multiplier.

### `draw_type`

Draw type of the block, see [Draw types](#draw-types) for more details.

Example:
```lua
draw_type = "solid" -- this is the default value
```

### `fog_color`

Color of the fog drawn for players inside the block.

Example:
```lua
fog_color = {255, 0, 255}
```

### `fog_depth`

Depth of the fog drawn for players inside the block.

Example:
```lua
fog_depth = 20
```

### `groups`

Groups of the block. They can be used in recipes, and can also filter Lua-defined inventory widgets.

See [Item#Attributes#groups](lua-api-item.md#groups) for more details.

### `hardness`

Hardness of the block, affects mining speed.

Example:
```lua
hardness = 0.5
```

Default value is `1.0`, any value under this will increase mining speed, and any value above will decrease mining speed.

### `harvest_requirements`

Set which tools are more effective.

**Note:** This attribute would need more doc but it'll probably get removed soon.

### `id`

ID of the block. **Mandatory field.**

Example:
```lua
id = "cobblestone"
```

IDs are usually of the form `mod:block` but the `mod:` prefix is prepended automatically so it's not needed.

### `inventory_image`

Texture used for inventory rendering.

Example:
```lua
inventory_image = "cobblestone_item.png"
```

If you set this parameter the block will be displayed as an item in the inventory using this texture.

The texture is loaded from `mods/<your-mod>/textures/items/`.

### `is_light_source`

Defines if the block is the light source or not.

Example:
```lua
is_light_source = false -- this is the default value
```

### `is_opaque`

Defines if the block let the light pass or not.

Example:
```lua
is_opaque = true -- this is the default value
```

### `is_rotatable`

Defines if the block should face the player when placed, like the furnace for example

Example:
```lua
is_rotatable = false -- this is the default value
```

### `item_drop`

Item stack that is given to the player when breaking the block.

Example:
```lua
item_drop = {
	id = "default:cobblestone",
	amount = 1
} -- this is the default drop of a 'default:cobblestone' block
```

### `name`

Label of the block. **Mandatory field.**

Example:
```lua
name = "Cobblestone"
```

This label is the name that will appear everywhere in the game.

### `tick_probability`

Probability to call the `on_tick` function when `tick_randomly` is enabled.

Example:
```lua
tick_probability = 0.5 -- 50%
```

### `tick_randomly`

Whether or not the block `on_tick` function is executed randomly.

See also `tick_probability`.

Example:
```lua
tick_randomly = true
```

### `tiles`

This field can be either a single string, or a table of strings.

Example:
``` lua
tiles = "myblock.png"
-- or
tiles = {"myblock_top.png", "myblock_bottom.png", "myblock_side.png"}
```

Table format:
```cpp
// namely: West, East, South, North, Bottom, Top
{0, 0, 0, 0, 0, 0}, // for size = 1
{1, 1, 1, 1, 0, 0}, // for size = 2
{2, 2, 2, 2, 1, 0}, // for size = 3
{3, 2, 3, 3, 1, 0}, // for size = 4
{3, 2, 4, 4, 1, 0}, // for size = 5
{3, 2, 4, 5, 1, 0}, // for size = 6
```

The textures will be loaded from `mods/<your-mod>/textures/blocks`

**Note:** Currently, you can only use textures of the exact same size (16x16, 32x32) than the other block/item textures in the game.

## Draw types

The draw type changes how the block is rendered.

### `boundingbox`

NB: Temporary draw type.

Allow drawing a block of the size of the `bounding_box`.

### `cactus`

Using the `bounding_box` attribute, will draw a # shape instead of a cube.

Useful for `cactus` block.

### `glass`

Draw type used for glass-type blocks. Also works for portals.

Almost the same as the `liquid` draw type, except that back-face culling is enabled.

### `leaves`

Keep all the faces while drawing a block.

Especially useful for leaves.

### `liquid`

This draw type doesn't do much yet.

Makes a single mesh with all the connected blocks of the same type in a chunk.

It will be rendered without back-face culling (this is why you can see water surface while being inside).

### `solid`

This draw type is the default draw type of the blocks.

It renders a basic textured cube of size 1x1x1.

### `xshape`

Draws two textured quads that form an X shape.

Useful for flora, tallgrass, mushrooms, etc...

## Functions

### `on_block_activated`

Parameters:

- `pos` (`ivec3`): position of the block
- `block` (`ServerBlock`): block definition
- `player` (`ServerPlayer`): player that activated the block
- `world` (`ServerWorld`): instance of the world
- `client` (`Client`): client that activated the block
- `server` (`Server`): current server
- `screen_width` (`u16`): width of the screen
- `screen_height` (`u16`): height of the screen
- `gui_scale` (`u8`): current scaling setting

### `on_block_destroyed`

Parameters:

- `pos` (`ivec3`): position of the block
- `world` (`ServerWorld`): instance of the world

### `on_block_placed`

Parameters:

- `pos` (`ivec3`): position of the block
- `world` (`ServerWorld`): instance of the world

### `on_tick`

Parameters:

- `pos` (`ivec3`): position of the block
- `block` (`ServerBlock`): block definition
- `chunk` (`ServerChunk`): current chunk
- `world` (`ServerWorld`): instance of the world

