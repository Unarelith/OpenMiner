# Lua API: GUI API Overview

## GUI creation

You can create a new GUI with:
```lua
local gui = LuaGUI.new()
```

## Set the size of the gui

```lua
gui:set_size(150, 40)
```

## Center the gui on the screen

**Note:** This depends on a correct size, defined with `gui:set_size`.

```lua
gui:set_centered(true)
```

## Showing the gui to a client

To show the custom GUI use:
```lua
gui:show(client)
```

