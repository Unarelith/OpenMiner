# Lua API: Blocks

- [Attributes](#attributes)
    - [Data types](#data-types)
    - [Draw types](#draw-types)
- [Functions](#functions)
    - [on_block_placed](#on_block_placed)
    - [on_block_activatd](#on_block_activated)
    - [on_tick](#on_tick)

## Attributes

Blocks can have the following attributes:

| Attribute            | Type          | Description                                          |
| -------------------- | ------------- | ---------------------------------------------------- |
| id*                  | string        | ID of the block without the "mod:" prefix            |
| label*               | string        | Label of the block                                   |
| on_block_placed      | function      | Called when the block is placed                      |
| on_block_activated   | function      | Called when a player right-click the block           |
| on_tick              | function      | Called every tick                                    |
| harvest_requirements | u8            | Set which tools are more effective                   |
| hardness             | float         | Hardness of the block, affects mining speed          |
| is_opaque            | bool          | Whether or not the block let light pass              |
| is_light_source      | bool          | Whether or not the block is a light source           |
| is_rotatable         | bool          | Whether or not the block is rotatable                |
| bounding_box         | float[6]      | Bounding box of the block {x,y,z,w,d,h}              |
| draw_type            | string        | Draw type of the block (see [Draw types](#draw-types)) |
| item_drop            | ItemStack     | Item stack that is dropped when the block is broken  |
| color_multiplier     | u8[4]         | Only grayscale colors are affected by this attribute |

NB: Attributes with a `*` means they're mandatory

### Data types

`ItemStack` is a table composed of:

- `id` (`string`): item id with the "mod:" prefix
- `amount` (`u16`): amount in the stack

### Draw types

The currently allowed draw type values are:

- `solid`
- `xshape`
- `leaves`
- `liquid`
- `glass`

## Functions

### on_block_placed

Parameters:

- `pos` (`vec3`): position of the block
- `world` (`World`): instance of `ServerWorld`

### on_block_activated

Parameters:

- `pos` (`vec3`): position of the block
- `player` (`Player`): current player
- `world` (`World`): instance of the `ServerWorld`
- `client` (`Client`): client that activated the block
- `screen_width` (`u16`): width of the screen
- `screen_height` (`u16`): height of the screen
- `gui_scale` (`u8`): current scaling setting

### on_tick

Parameters:

- `pos` (`vec3`): position of the block
- `player` (`Player`): current player
- `chunk` (`Chunk`): current chunk
- `world` (`World`): instance of the `ServerWorld`

