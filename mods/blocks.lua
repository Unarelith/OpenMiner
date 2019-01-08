mod:block {
	id = "",
	name = "Air",
	texture = 0,
}

mod:block {
	id = "dirt",
	name = "Dirt",
	texture = 37,
}

mod:block {
	id = "cobblestone",
	name = "Cobblestone",
	texture = 38,
	hardness = 2,
	harvest_requirements = 1,
}

mod:block {
	id = "grass",
	name = "Grass",
	texture = 226,
	item_drop = {
		id = mod:id()..":dirt",
		amount = 1
	}
}

mod:block {
	id = "leaves",
	name = "Leaves",
	texture = 266,
	hardness = 0.5,
}

mod:block {
	id = "wood",
	name = "Wood",
	texture = 277,
	hardness = 2
}

mod:block {
	id = "stone",
	name = "Stone",
	texture = 402,
	hardness = 1.5,
	harvest_requirements = 1,
	item_drop = {
		id = mod:id()..":cobblestone",
		amount = 1
	},
}

mod:block {
	id = "sand",
	name = "Sand",
	texture = 369,
}

mod:block {
	id = "water",
	name = "Water",
	texture = 457,
}

mod:block {
	id = "glass",
	name = "Glass",
	texture = 168,
}

mod:block {
	id = "ore_coal",
	name = "Coal Ore",
	texture = 36,
	hardness = 3,
	harvest_requirements = 1,
	item_drop = {
		id = mod:id()..":coal",
		amount = 1
	},
}

mod:block {
	id = "planks",
	name = "Planks",
	texture = 316,
}

mod:block {
	id = "glowstone",
	name = "Glowstone",
	texture = 218,
}

mod:block {
	id = "workbench",
	name = "Workbench",
	texture = 77,

	on_block_activated = function(pos, player, world)
		local gui = LuaGUI.new()

		-- FIXME: Replace this by gui:set_size() and gui:set_centered()
		local gui_pos = {
			x = SCREEN_WIDTH / GUI_SCALE / 2.0 - 176 / 2.0,
			y = SCREEN_HEIGHT / GUI_SCALE / 2.0 - 166 / 2.0
		}

		gui:button {
			name = "btn_hello",
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

		gui:show()
	end,
}

mod:block {
	id = "furnace",
	name = "Furnace",
	texture = 164,

	on_block_activated = function(pos, player, world)
		-- FIXME: Replace this by gui:set_size() and gui:set_centered()
		local gui_pos = {
			x = SCREEN_WIDTH / GUI_SCALE / 2.0 - 176 / 2.0,
			y = SCREEN_HEIGHT / GUI_SCALE / 2.0 - 166 / 2.0
		}

		local gui = LuaGUI.new()
		gui:furnace {
			name = "furnace",
			pos = gui_pos,
			block = {x = pos.x, y = pos.y, z = pos.z}
		}
		gui:show()
	end,

	on_tick = function(pos, player, chunk, world)
		local data = world:get_block_data(pos.x, pos.y, pos.z)
		if not data then return end

		local input_stack = data.inventory:get_stack(0, 0)
		local output_stack = data.inventory:get_stack(1, 0)
		local fuel_stack = data.inventory:get_stack(2, 0)

		local ticks_remaining = bit.band(data.data, 0xfff)
		local current_burn_time = bit.band(bit.brshift(data.data, 12), 0xfff)
		local item_progress = bit.band(bit.brshift(data.data, 24), 0xff)

		-- print(tostring(ticks_remaining).." "..tostring(current_burn_time).." "..tostring(item_progress))

		local recipe = openminer:registry():get_recipe(data.inventory)
		if recipe and recipe:type() ~= "smelt" then
			recipe = nil
		end

		if ticks_remaining == 0 and recipe and fuel_stack:amount() > 0 and
				(output_stack:item():id() == 0 or output_stack:amount() == 0
				or output_stack:item():id() == recipe:result():item():id()) then
			ticks_remaining = fuel_stack:item():burn_time()
			current_burn_time = fuel_stack:item():burn_time()
			world:set_data(pos.x, pos.y, pos.z, 1)
		elseif ticks_remaining > 0 then
			ticks_remaining = ticks_remaining - 1

			if recipe and (output_stack:item():id() == 0 or output_stack:amount() == 0
					or output_stack:item():id() == recipe:result():item():id()) then
				item_progress = item_progress + 1
			else
				item_progress = 0
			end
		elseif ticks_remaining == 0 then
			current_burn_time = 0
			world:set_data(pos.x, pos.y, pos.z, 0)
		end

		if item_progress >= 200 and recipe then
			item_progress = 0;
			data.inventory:set_stack(0, 0, (input_stack:amount() - 1 > 0) and input_stack:item():name() or "", input_stack:amount() - 1)
			data.inventory:set_stack(1, 0, recipe:result():item():name(), output_stack:amount() + recipe:result():amount())
		end

		local a = bit.blshift(bit.band(current_burn_time, 0xfff), 12)
		local b = bit.blshift(bit.band(item_progress, 0xff), 24)
		data.data = bit.band(ticks_remaining, 0xfff) + a + b
	end,
}

mod:block {
	id = "ore_iron",
	name = "Iron Ore",
	texture = 254,
	hardness = 3,
	harvest_requirements = 1,
}

mod:block {
	id = "slab_planks",
	name = "Plank Slab",
	texture = 316,
}

mod:block {
	id = "flower",
	name = "Flower",
	texture = 150,
	hardness = 0.05,
	draw_type = 1,
}

for i = 18, 31 do
	mod:block {
		id = "undefined" .. i,
		name = "Undefined",
		texture = 4,
	}
end

