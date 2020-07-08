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

mod:key {
	id = "inventory",
	name = "Inventory",
	default_key = "E",

	callback = function(keyID, client, screen_width, screen_height, gui_scale)
		local gui = LuaGUI.new(keyID)

		gui:set_size(176, 166)
		gui:set_centered(true)

		gui:image {
			name = "img_background",
			pos = {x = 0, y = 0},

			texture = modpath .. "/textures/gui/inventory.png",
			clip = {x = 0, y = 0, width = 176, height = 166},
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

			shift_destination = "inv_hotbar,inv_main",
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

			shift_destination = "inv_main,inv_hotbar",
		}

		gui:crafting {
			name = "inv_crafting",
			pos = {x = 97, y = 17},
			result_pos = {x = 97 + 56, y = 17 + 10},

			inventory = {
				source = "temp",
				size = 2,
			},

			shift_destination = "inv_main,inv_hotbar",
		}

		gui:show(client)
	end
}

