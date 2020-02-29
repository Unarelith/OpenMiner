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
mod = LuaMod.new("default")

dofile("mods/default/blocks.lua")
dofile("mods/default/items.lua")
dofile("mods/default/recipes.lua")

openminer:world():terrain_generator():set_blocks({
	dirt = "default:dirt",
	grass = "default:grass",
	stone = "default:stone",
	log = "default:wood",
	leaves = "default:leaves",
	flower = "default:flower",
	water = "default:water",
	sand = "default:sand",
	tallgrass = "default:tallgrass",
	iron_ore = "default:iron_ore"
})

function init(player)
	local player_inv = player:inventory()

	player_inv:add_stack("default:workbench", 1);
	player_inv:add_stack("default:dirt", 64);
	player_inv:add_stack("default:grass", 64);
	player_inv:add_stack("default:stone", 64);
	player_inv:add_stack("default:glass", 64);
	player_inv:add_stack("default:glowstone", 64);
	player_inv:add_stack("default:furnace", 1);
	player_inv:add_stack("default:stone_pickaxe", 1);
	player_inv:add_stack("default:stone_axe", 1);

	player_inv:add_stack("default:wood", 64);
	player_inv:add_stack("default:planks", 64);
	player_inv:add_stack("default:cobblestone", 64);
	player_inv:add_stack("default:stick", 64);
	player_inv:add_stack("default:stone_hoe", 1);
	player_inv:add_stack("default:stone_shovel", 1);
	player_inv:add_stack("default:iron_ore", 64);
	player_inv:add_stack("default:coal", 64);

	player_inv:add_stack("default:iron_ingot", 64);
	player_inv:add_stack("default:gold_ingot", 64);
	player_inv:add_stack("default:diamond", 64);
end

function show_inventory(client, screen_width, screen_height, gui_scale)
	local gui = LuaGUI.new()

	-- FIXME: Replace this by gui:set_size() and gui:set_centered()
	local gui_pos = {
		x = math.floor(screen_width / gui_scale / 2.0 - 176 / 2.0 + 0.5),
		y = math.floor(screen_height / gui_scale / 2.0 - 166 / 2.0 + 0.5)
	}

	gui:image {
		name = "img_background",
		pos = gui_pos,

		texture = "texture-inventory",
		clip = {x = 0, y = 0, width = 176, height = 166},
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
	}

	gui:crafting {
		name = "inv_crafting",
		pos = {x = gui_pos.x + 97, y = gui_pos.y + 17},
		result_pos = {x = gui_pos.x + 97 + 56, y = gui_pos.y + 17 + 10},

		inventory = "temp",
		size = 2,
	}

	gui:show(client)
end
