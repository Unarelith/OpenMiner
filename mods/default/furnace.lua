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
	is_rotatable = true,

	on_block_placed = function(pos, world)
		world:add_block_data(pos.x, pos.y, pos.z, 3, 1)
	end,

	on_block_activated = function(pos, player, world, client, server, screen_width, screen_height, gui_scale)
		local gui = LuaGUI.new()

		gui:set_size(176, 166)
		gui:set_centered(true)

		gui:progress_bar {
			name = "bar_process",
			pos = {x = 80, y = 35},

			type = "item_process",

			block = {x = pos.x, y = pos.y, z = pos.z},

			meta = "item_progress",
			max_value = 200,

			texture = "mods/default/textures/gui/furnace.png",
			clip = {x = 176, y = 14, width = 24, height = 17},
		}

		gui:progress_bar {
			name = "bar_burn",
			pos = {x = 57, y = 37},

			type = "burn_process",

			block = {x = pos.x, y = pos.y, z = pos.z},

			meta = "ticks_remaining",
			max_meta = "current_burn_time",

			texture = "mods/default/textures/gui/furnace.png",
			clip = {x = 176, y = 0, width = 14, height = 14},
		}

		gui:inventory {
			name = "inv_input",
			pos = {x = 55, y = 16},

			inventory = {
				source = "block",
				block = {x = pos.x, y = pos.y, z = pos.z},
				offset = 0,
				count = 1,
			},

			size = {x = 1, y = 1},

			shift_destination = "inv_main,inv_hotbar",
		}

		gui:inventory {
			name = "inv_output",
			pos = {x = 115, y = 34},

			inventory = {
				source = "block",
				block = {x = pos.x, y = pos.y, z = pos.z},
				offset = 1,
				count = 1,
			},

			size = {x = 1, y = 1},

			shift_destination = "inv_main,inv_hotbar",
		}

		gui:inventory {
			name = "inv_fuel",
			pos = {x = 55, y = 52},

			inventory = {
				source = "block",
				block = {x = pos.x, y = pos.y, z = pos.z},
				offset = 2,
				count = 1,
			},

			size = {x = 1, y = 1},

			filter = "group:om_fuel",
			shift_destination = "inv_main,inv_hotbar",
		}

		gui:inventory {
			name = "inv_main",
			pos = {x = 7, y = 83},

			inventory = {
				source = "player",
				player = "player",
				inventory_name = "main",
				offset = 9,
				count = 9 * 3,
			},

			size = {x = 9, y = 3},

			shift_destination = "inv_input,inv_fuel",
		}

		gui:inventory {
			name = "inv_hotbar",
			pos = {x = 7, y = 141},

			inventory = {
				source = "player",
				player = "player",
				inventory_name = "main",
				offset = 0,
				count = 9,
			},

			size = {x = 9, y = 1},

			shift_destination = "inv_input,inv_fuel",
		}

		gui:image {
			name = "img_background",
			pos = {x = 0, y = 0},

			texture = "mods/default/textures/gui/furnace.png",
			clip = {x = 0, y = 0, width = 176, height = 166},
		}

		gui:show(client)
	end,

	on_tick = function(pos, chunk, world)
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
				fuel_stack:item():has_group("om_fuel") and
				(output_stack:item():id() == 0 or output_stack:amount() == 0
				or output_stack:item():id() == recipe:result():item():id()) then
			data.inventory:set_stack(2, 0, fuel_stack:item():string_id(), fuel_stack:amount() - 1)
			ticks_remaining = fuel_stack:item():get_group_value("om_fuel")
			current_burn_time = fuel_stack:item():get_group_value("om_fuel")
			data.use_alt_tiles = true;
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
			data.use_alt_tiles = false;
			-- world:set_data(pos.x, pos.y, pos.z, 0)
		end

		if item_progress >= 200 and recipe then
			item_progress = 0;
			data.inventory:set_stack(0, 0, (input_stack:amount() - 1 > 0) and input_stack:item():string_id() or "", input_stack:amount() - 1)
			data.inventory:set_stack(1, 0, recipe:result():item():string_id(), output_stack:amount() + recipe:result():amount())
		end

		data.meta:set_int("ticks_remaining", ticks_remaining);
		data.meta:set_int("current_burn_time", current_burn_time);
		data.meta:set_int("item_progress", item_progress);
	end,
}
