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
if not mods then mods = {} end

mods["creative_inventory"] = openminer.mod_loader:register_mod("creative_inventory")
mod = mods["creative_inventory"]

local modpath = mod:path()

-- FIXME: Put it in the engine if it works with tables too
local function _ipairs(t, var)
	var = var + 1
	local value = t[var]
	if value == nil then return end
	return var, value
end
function ipairs(t) return _ipairs, t, 0 end

mod:key {
	id = "creative_inventory",
	name = "Creative window",
	default_key = "H",

	callback = function(keyID, client, screen_width, screen_height, gui_scale)
		items = {}
		for k, v in ipairs(openminer.registry:items()) do
			if k ~= 1 and not v:has_group("group:ci_ignore") then
				items[#items + 1] = {v:string_id()}
			end
		end

		local gui = LuaGUI.new(keyID)

		gui:set_size(195, 136)
		gui:set_centered(true)

		gui:image {
			name = "img_background",
			pos = {x = 0, y = 0},

			texture = modpath .. "/textures/gui/creative_window.png",
			clip = {x = 0, y = 0, width = 195, height = 136},
		}

		gui:inventory_data {
			name = "inv_data",

			width = 9,
			height = math.ceil(#items / 9),

			items = items,

			is_unlimited = true,
		}

		gui:inventory {
			name = "inv_creative_items",
			pos = {x = 8, y = 17},

			inventory = {
				source = "temp",
				inventory_name = "inv_data",
				offset = 0,
				count = 9 * 5,
			},

			size = {x = 9, y = 7}
		}

		gui:text_input {
			name = "inv_input_filter",
			pos = {x = 82, y = 6},

			width = 86,
			height = 8,

			placeholder = "Search...",
			placeholder_color = {192, 192, 192},

			inventory = "inv_creative_items"
		}

		gui:scroll_bar {
			name = "scroll_bar",
			pos = {x = 175, y = 18},

			texture = modpath .. "/textures/gui/tabs.png",
			clip = {x = 232, y = 0, width = 12, height = 15},
			clip_selected = {x = 244, y = 0, width = 12, height = 15},

			widget = "inv_creative_items",

			min_y = 0,
			max_y = 110 - 15,
		}

		gui:inventory {
			name = "inv_hotbar",
			pos = {x = 8, y = 111},

			inventory = {
				source = "player",
				player = "player",
				inventory_name = "main",
				offset = 0,
				count = 9,
			},

			size = {x = 9, y = 1},
		}

		gui:show(client);
	end
}

