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

-- openminer:add_listener(Event.BlockPlaced, function(pos, block, player, world, client, server)
-- 	server:send_chat_message(0, "Block placed at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client)
-- end)

-- openminer:add_listener(Event.BlockDigged, function(pos, block, player, world, client, server)
-- 	server:send_chat_message(0, "Block digged at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client)
-- end)

openminer:add_listener(Event.BlockActivated, function(pos, block, player, world, client, server)
	if block:string_id() == "default:portal" then
		server:send_chat_message(0, "Swoosh! Changing dimension...", client)
	end
end)

openminer:add_listener(Event.PlayerConnected, function(pos, player, client, server)
	local starting_items = {
		{"default:workbench",       1},
		{"default:dirt",           64},
		{"default:grass",          64},
		{"default:stone",          64},
		{"default:glass",          64},
		{"default:glowstone",      64},
		{"default:furnace",         1},
		{"default:stone_pickaxe",   1},
		{"default:stone_axe",       1},

		{"default:oak_wood",       64},
		{"default:oak_planks",     64},
		{"default:cobblestone",    64},
		{"default:stick",          64},
		{"default:stone_hoe",       1},
		{"default:stone_shovel",    1},
		{"default:iron_ore",       64},
		{"default:coal",           64},

		{"default:iron_ingot",     64},
		{"default:gold_ingot",     64},
		{"default:diamond",        64}
	}


	local player_inv = player:inventory()
	for _, v in ipairs(starting_items) do
		player_inv:add_stack(v[1], v[2])
	end

	server:send_chat_message(0, "Welcome to OpenMiner!", client)
end)

