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
dofile("mods/default/sky.lua")
dofile("mods/default/trees.lua")
dofile("mods/default/biomes.lua")
dofile("mods/default/dimensions.lua")

-- openminer:add_listener(EventType.OnBlockPlaced, function(pos, player, world, client, server)
-- 	server:send_chat_message(0, "Block placed at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client);
-- end)

openminer:add_listener(EventType.OnBlockActivated, function(pos, block, player, world, client, server)
	if block:string_id() == "default:portal" then
		server:send_chat_message(0, "Swoosh! Changing dimension...", client);
	end
end)

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

	player_inv:add_stack("default:oak_wood", 64);
	player_inv:add_stack("default:oak_planks", 64);
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

	gui:set_size(176, 166)
	gui:set_centered(true)

	gui:image {
		name = "img_background",
		pos = {x = 0, y = 0},

		texture = "mods/default/textures/gui/inventory.png",
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

