mod:block {
	id = "workbench",
	name = "Workbench",
	-- texture = 77, -- Vanilla
	texture = 115, -- Faithful 32x

	on_block_activated = function(pos, player, world, client)
		local gui = LuaGUI.new()

		-- FIXME: Replace this by gui:set_size() and gui:set_centered()
		local gui_pos = {
			x = SCREEN_WIDTH / GUI_SCALE / 2.0 - 176 / 2.0,
			y = SCREEN_HEIGHT / GUI_SCALE / 2.0 - 166 / 2.0
		}

		gui:button {
			name = "btn_test",
			pos = {x = 0, y = 0},

			text = "Test button",
			on_click = function(self)
				print("Test button pressed")
			end,
		}

		gui:inventory {
			name = "inv_main",
			pos = {x = gui_pos.x + 7, y = gui_pos.y + 83},

			player = "player",
			inventory = "main",
			size = {x = 9, y = 3},
			offset = 9,
			count = 9 * 3,
		}

		gui:inventory {
			name = "inv_hotbar",
			pos = {x = gui_pos.x + 7, y = gui_pos.y + 141},

			player = "player",
			inventory = "main",
			size = {x = 9, y = 1},
			offset = 0,
			count = 9,
		}

		gui:crafting {
			name = "inv_crafting",
			pos = {x = gui_pos.x, y = gui_pos.y},

			block = {x = pos.x, y = pos.y, z = pos.z},

			offset = 0,
		}

		gui:image {
			name = "img_background",
			pos = gui_pos,

			texture = "texture-workbench",
			clip = {x = 0, y = 0, width = 176, height = 166},
		}

		gui:show(client)
	end,
}
