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
	id = "workbench",
	name = "Workbench",
	tiles = {"crafting_table_top.png", "crafting_table_top.png", "crafting_table_front.png", "crafting_table_side.png"},

	on_block_placed = function(pos, world)
		world:add_block_data(pos.x, pos.y, pos.z, 3, 3)
	end,

	on_block_activated = function(pos, player, world, client, screen_width, screen_height, gui_scale)
		local gui = LuaGUI.new()

		-- FIXME: Replace this by gui:set_size() and gui:set_centered()
		local gui_pos = {
			x = math.floor(screen_width / gui_scale / 2.0 - 176 / 2.0 + 0.5),
			y = math.floor(screen_height / gui_scale / 2.0 - 166 / 2.0 + 0.5)
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

			shift_destination = "inv_crafting",
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

			shift_destination = "inv_crafting",
		}

		gui:crafting {
			name = "inv_crafting",
			pos = {x = gui_pos.x + 29, y = gui_pos.y + 16},
			result_pos = {x = gui_pos.x + 123, y = gui_pos.y + 34},

			inventory = "block",
			block = {x = pos.x, y = pos.y, z = pos.z},

			shift_destination = "inv_main",
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
