--
-- =====================================================================================
--
--  OpenMiner
--
--  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
--  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
--
--  This file is part of OpenMiner.
--
--  OpenMiner is free software; you can redistribute it and/or
--  modify it under the terms of the GNU Lesser General Public
--  License as published by the Free Software Foundation; either
--  version 2.1 of the License, or (at your option) any later version.
--
--  OpenMiner is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
--  Lesser General Public License for more details.
--
--  You should have received a copy of the GNU Lesser General Public License
--  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
--  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
--
-- =====================================================================================
--

mod:block {
	id = "furnace",
	name = "Furnace",
	tiles = {"furnace_top.png", "furnace_top.png", "furnace_front.png", "furnace_side.png"},
	alt_tiles = {"", "", "furnace_front_on.png", ""},

	on_block_placed = function(pos, world)
		world:add_block_data(pos.x, pos.y, pos.z, 3, 1)
	end,

	on_block_activated = function(pos, player, world, client, screen_width, screen_height, gui_scale)
		local gui = LuaGUI.new()

		-- FIXME: Replace this by gui:set_size() and gui:set_centered()
		local gui_pos = {
			x = math.floor(screen_width / gui_scale / 2.0 - 176 / 2.0 + 0.5),
			y = math.floor(screen_height / gui_scale / 2.0 - 166 / 2.0 + 0.5)
		}

		gui:progress_bar {
			name = "bar_process",
			pos = {x = gui_pos.x + 80, y = gui_pos.y + 35},

			type = 0, -- FIXME: Use string for that

			block = {x = pos.x, y = pos.y, z = pos.z},

			meta = "item_progress",
			max_value = 200,

			texture = "texture-furnace",
			clip = {x = 176, y = 14, width = 24, height = 17},
		}

		gui:progress_bar {
			name = "bar_burn",
			pos = {x = gui_pos.x + 57, y = gui_pos.y + 37},

			type = 1, -- FIXME: Use string for that

			block = {x = pos.x, y = pos.y, z = pos.z},

			meta = "ticks_remaining",
			max_meta = "current_burn_time",

			texture = "texture-furnace",
			clip = {x = 176, y = 0, width = 14, height = 14},
		}

		gui:inventory {
			name = "inv_input",
			pos = {x = gui_pos.x + 55, y = gui_pos.y + 16},

			inventory = "block",
			block = {x = pos.x, y = pos.y, z = pos.z},

			size = {x = 1, y = 1},
			offset = 0,
			count = 1,

			shift_destination = "inv_main,inv_hotbar",
		}

		gui:inventory {
			name = "inv_output",
			pos = {x = gui_pos.x + 115, y = gui_pos.y + 34},

			inventory = "block",
			block = {x = pos.x, y = pos.y, z = pos.z},

			size = {x = 1, y = 1},
			offset = 1,
			count = 1,

			shift_destination = "inv_main,inv_hotbar",
		}

		gui:inventory {
			name = "inv_fuel",
			pos = {x = gui_pos.x + 55, y = gui_pos.y + 52},

			inventory = "block",
			block = {x = pos.x, y = pos.y, z = pos.z},

			size = {x = 1, y = 1},
			offset = 2,
			count = 1,

			shift_destination = "inv_main,inv_hotbar",
		}

		gui:inventory {
			name = "inv_main",
			pos = {x = gui_pos.x + 7, y = gui_pos.y + 83},

			inventory = "player",
			player = "player",
			inventory_name = "main",

			size = {x = 9, y = 3},
			offset = 9,
			count = 9 * 3,

			shift_destination = "inv_input,inv_fuel",
		}

		gui:inventory {
			name = "inv_hotbar",
			pos = {x = gui_pos.x + 7, y = gui_pos.y + 141},

			inventory = "player",
			player = "player",
			inventory_name = "main",

			size = {x = 9, y = 1},
			offset = 0,
			count = 9,

			shift_destination = "inv_input,inv_fuel",
		}

		gui:image {
			name = "img_background",
			pos = gui_pos,

			texture = "texture-furnace",
			clip = {x = 0, y = 0, width = 176, height = 166},
		}

		gui:show(client)
	end,

	on_tick = function(pos, player, chunk, world)
		local data = world:get_block_data(pos.x, pos.y, pos.z)
		if not data then return end

		local input_stack = data.inventory:get_stack(0, 0)
		local output_stack = data.inventory:get_stack(1, 0)
		local fuel_stack = data.inventory:get_stack(2, 0)

		local ticks_remaining = data.meta:get_int("ticks_remaining") or 0
		local current_burn_time = data.meta:get_int("current_burn_time") or 0
		local item_progress = data.meta:get_int("item_progress") or 0

		local recipe = openminer:registry():get_recipe(data.inventory)
		if recipe and recipe:type() ~= "smelt" then
			recipe = nil
		end

		if ticks_remaining == 0 and recipe and fuel_stack:amount() > 0 and
				(output_stack:item():id() == 0 or output_stack:amount() == 0
				or output_stack:item():id() == recipe:result():item():id()) then
			data.inventory:set_stack(2, 0, fuel_stack:item():stringID(), fuel_stack:amount() - 1)
			ticks_remaining = fuel_stack:item():burn_time()
			current_burn_time = fuel_stack:item():burn_time()
			data.useAltTiles = true;
			-- world:set_data(pos.x, pos.y, pos.z, 1)
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
			data.useAltTiles = false;
			-- world:set_data(pos.x, pos.y, pos.z, 0)
		end

		if item_progress >= 200 and recipe then
			item_progress = 0;
			data.inventory:set_stack(0, 0, (input_stack:amount() - 1 > 0) and input_stack:item():stringID() or "", input_stack:amount() - 1)
			data.inventory:set_stack(1, 0, recipe:result():item():stringID(), output_stack:amount() + recipe:result():amount())
		end

		data.meta:set_int("ticks_remaining", ticks_remaining);
		data.meta:set_int("current_burn_time", current_burn_time);
		data.meta:set_int("item_progress", item_progress);
	end,
}
