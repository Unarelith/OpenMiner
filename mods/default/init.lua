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
dofile("mods/default/trees.lua")
dofile("mods/default/biomes.lua")
dofile("mods/default/dimensions.lua")

function init(player)
	local player_inv = player:inventory()

	player_inv:add_stack("default:debug", 64);
	player_inv:add_stack("default:dirt", 64);
	player_inv:add_stack("default:grass", 64);
	player_inv:add_stack("default:stone", 64);
	player_inv:add_stack("default:glass", 64);
	player_inv:add_stack("default:glowstone", 64);
	player_inv:add_stack("default:workbench", 1);
	player_inv:add_stack("default:furnace", 1);
	player_inv:add_stack("default:stone_pickaxe", 1);

	player_inv:add_stack("default:oak_wood", 64);
	player_inv:add_stack("default:oak_planks", 64);
	player_inv:add_stack("default:cobblestone", 64);
	player_inv:add_stack("default:stick", 64);
	player_inv:add_stack("default:stone_hoe", 1);
	player_inv:add_stack("default:stone_shovel", 1);
	player_inv:add_stack("default:stone_axe", 1);
	player_inv:add_stack("default:iron_ore", 64);
	player_inv:add_stack("default:coal", 64);
	player_inv:add_stack("default:oak_slab", 64);
	player_inv:add_stack("default:portal", 64);

	player_inv:add_stack("default:iron_ingot", 64);
	player_inv:add_stack("default:gold_ingot", 64);
	player_inv:add_stack("default:diamond", 64);
end

function show_inventory(client, screen_width, screen_height, gui_scale)
	local gui = LuaGUI.new()

	gui:set_size(176, 166)
	gui:set_centered(true)

	gui:image {
		name = "img_background",
		pos = {x = 0, y = 0},

		texture = "texture-inventory",
		clip = {x = 0, y = 0, width = 176, height = 166},
	}

	gui:inventory {
		name = "inv_main",
		pos = {x = 7, y = 83},

		inventory = "player",
		player = "player",
		inventory_name = "main",
		size = {x = 9, y = 3},
		offset = 9,
		count = 9 * 3,

		shift_destination = "inv_hotbar,inv_main",
	}

	gui:inventory {
		name = "inv_hotbar",
		pos = {x = 7, y = 141},

		inventory = "player",
		player = "player",
		inventory_name = "main",
		size = {x = 9, y = 1},
		offset = 0,
		count = 9,

		shift_destination = "inv_main,inv_hotbar",
	}

	gui:crafting {
		name = "inv_crafting",
		pos = {x = 97, y = 17},
		result_pos = {x = 97 + 56, y = 17 + 10},

		inventory = "temp",
		size = 2,

		shift_destination = "inv_main,inv_hotbar",
	}

	gui:show(client)
end

function show_creative_window(client, screen_width, screen_height, gui_scale)
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
		height = 5,

		items = {
			-- Blocks
			{"default:dirt"},
			{"default:cobblestone"},
			{"default:grass"},
			{"default:oak_leaves"},
			{"default:oak_wood"},
			{"default:stone"},
			{"default:sand"},
			{"default:water"},
			{"default:glass"},
			{"default:coal_ore"},
			{"default:oak_planks"},
			{"default:glowstone"},
			{"default:workbench"},
			{"default:furnace"},
			{"default:iron_ore"},
			{"default:dandelion"},
			{"default:tallgrass"},
			{"default:stone_bricks"},
			{"default:bricks"},
			{"default:clay"},
			{"default:oak_slab"},
			{"default:portal"},
			{"default:netherrack"},
			{"default:soul_sand"},
			{"default:lava"},
			{"default:cactus"},
			{"default:deadbush"},
			{"default:debug"},

			-- Items
			{"default:stick"},
			-- {"default:stone_axe"},
			-- {"default:stone_hoe"},
			-- {"default:stone_pickaxe"},
			-- {"default:stone_shovel"},
			-- {"default:stone_sword"},
			{"default:coal"},
			{"default:iron_ingot"},
			{"default:charcoal"},
			-- {"default:wooden_axe"},
			-- {"default:wooden_hoe"},
			-- {"default:wooden_pickaxe"},
			-- {"default:wooden_shovel"},
			-- {"default:wooden_sword"},
			{"default:brick"},
			{"default:clay_ball"},
			{"default:diamond"},
			{"default:gold_ingot"},
			-- {"default:iron_pickaxe"},
			-- {"default:iron_sword"},
			-- {"default:iron_axe"},
			-- {"default:iron_hoe"},
			-- {"default:iron_shovel"},
			-- {"default:diamond_pickaxe"},
			-- {"default:diamond_sword"},
			-- {"default:diamond_axe"},
			-- {"default:diamond_hoe"},
			-- {"default:diamond_shovel"},
			-- {"default:golden_pickaxe"},
			-- {"default:golden_sword"},
			-- {"default:golden_axe"},
			-- {"default:golden_hoe"},
			-- {"default:golden_shovel"},
		},

		is_unlimited = true,
	}

	gui:inventory {
		name = "inv_creative_items",
		pos = {x = 8, y = 17},

		inventory = "temp",
		inventory_name = "inv_data",
		size = {x = 9, y = 5},

		offset = 0,
		count = 9 * 5,
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
