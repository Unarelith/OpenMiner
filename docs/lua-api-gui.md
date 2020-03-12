# Lua API: GUI API Overview

**Note:** This page is possibly outdated for the moment.

You can create a new GUI with:
```lua
local gui = LuaGUI.new()
```

To show the custom GUI use:
```lua
gui:show(client)
```

## Button

```lua
gui:button {
	name = "btn_test",
	pos = {x = 0, y = 0},

	text = "Test button",
	on_click = function(self)
		print("Test button pressed")
	end,
}
```

## Inventory

```lua
gui:inventory {
	name = "inv_main",
	pos = {x = gui_pos.x + 7, y = gui_pos.y + 83},

	player = "player",
	inventory = "main",
	size = {x = 9, y = 3},
	offset = 9,
	count = 9 * 3,
}
```

## Crafting table

```lua
gui:crafting {
	name = "inv_crafting",
	pos = {x = gui_pos.x, y = gui_pos.y},

	block = {x = pos.x, y = pos.y, z = pos.z},

	offset = 0,
}
```

## Image

```lua
gui:image {
	name = "img_background",
	pos = gui_pos,

	texture = "texture-workbench",
	clip = {x = 0, y = 0, width = 176, height = 166},
}
```

