# Lua API: Key

## Example

```lua
mod:key {
	id = "inventory",
	name = "Inventory",
	default_key = "E",

	callback = function(keyID, client, screen_width, screen_height, gui_scale)
		show_inventory(keyID, client, screen_width, screen_height, gui_scale)
	end
}
```

## Attributes

### `callback`

Function called when the key is pressed.

Example:
```lua
callback = function(keyID, client, screen_width, screen_height, gui_scale)
	show_inventory(keyID, client, screen_width, screen_height, gui_scale)
end
```

### `default_key`

Keyboard key mapped to this key by default.

Example:
```lua
default_key = "E"
```

Names are defined [here](https://wiki.libsdl.org/SDL_Keycode).

### `name`

Name of the key. Optional field, uses `id` if not defined.

Example:
```lua
name = "Inventory"
```

### `id`

ID of the key. **Mandatory field.**

Example:
```lua
id = "inventory"
```

IDs are usually of the form `mod:key` but the `mod:` prefix is prepended automatically so it's not needed.

