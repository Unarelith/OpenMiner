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

function show_creative_window(client, screen_width, screen_height, gui_scale)
	items = {}
	for k, v in pairs(openminer:registry():items()) do
		if k ~= 1 then
			items[#items + 1] = {v:string_id()}
		end
	end

	local gui = LuaGUI.new()

	gui:set_size(195, 136)
	gui:set_centered(true)

	gui:image {
		name = "img_background",
		pos = {x = 0, y = 0},

		texture = "texture-creative_window",
		clip = {x = 0, y = 0, width = 195, height = 136},
	}

	gui:inventory_data {
		name = "inv_data",

		width = 9,
		height = 7,

		items = items,

		is_unlimited = true,
	}

	gui:inventory {
		name = "inv_creative_items",
		pos = {x = 8, y = 17},

		inventory = "temp",
		inventory_name = "inv_data",

		offset = 0,
		count = 9 * 5,
	}

	gui:scroll_bar {
		name = "scroll_bar",
		pos = {x = 175, y = 18},

		texture = "texture-tabs",
		clip = {x = 232, y = 0, width = 12, height = 15},
		clip_selected = {x = 244, y = 0, width = 12, height = 15},

		widget = "inv_creative_items",

		min_y = 0,
		max_y = 110 - 15,
	}

	gui:inventory {
		name = "inv_hotbar",
		pos = {x = 8, y = 111},

		inventory = "player",
		player = "player",
		inventory_name = "main",
		size = {x = 9, y = 1},
		offset = 0,
		count = 9,
	}

	gui:show(client);
end

