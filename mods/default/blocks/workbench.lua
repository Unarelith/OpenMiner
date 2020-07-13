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
local modpath = mod:path()

mod:block {
	id = "workbench",
	name = "Workbench",
	tiles = {"crafting_table_top.png", "crafting_table_top.png", "crafting_table_front.png", "crafting_table_side.png"},
	groups = {
		om_material_wood = 1
	},

	hardness = 1,
	harvest_requirements = 4,

	on_block_placed = function(pos, world)
		world:add_block_data(pos.x, pos.y, pos.z, 3, 3)
	end,

	on_block_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		local gui = LuaGUI.new()

		gui:set_size(176, 166)
		gui:set_centered(true)

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

			shift_destination = "inv_crafting",
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

			shift_destination = "inv_crafting",
		}

		gui:crafting {
			name = "inv_crafting",
			pos = {x = 29, y = 16},
			result_pos = {x = 123, y = 34},

			inventory = {
				source = "block",
				block = {x = pos.x, y = pos.y, z = pos.z},
			},

			shift_destination = "inv_main,inv_hotbar",
		}

		gui:image {
			name = "img_background",
			pos = {x = 0, y = 0},

			texture = modpath .. "/textures/gui/workbench.png",
			clip = {x = 0, y = 0, width = 176, height = 166},
		}

		gui:show(client)
	end,
}
