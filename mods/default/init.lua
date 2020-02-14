--
-- =====================================================================================
--
--  OpenMiner
--  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
--
--  This program is free software; you can redistribute it and/or
--  modify it under the terms of the GNU Lesser General Public
--  License as published by the Free Software Foundation; either
--  version 2.1 of the License, or (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
--  Lesser General Public License for more details.
--
--  You should have received a copy of the GNU Lesser General Public License
--  along with this program; if not, write to the Free Software Foundation, Inc.,
--  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
--
-- =====================================================================================
--

-- FIXME
SCREEN_WIDTH = 1600
SCREEN_HEIGHT = 1050
GUI_SCALE = 3

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
	sand = "default:sand"
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
end

function show_inventory(client)
	local gui = LuaGUI.new()

	-- FIXME: Replace this by gui:set_size() and gui:set_centered()
	local gui_pos = {
		x = SCREEN_WIDTH / GUI_SCALE / 2.0 - 176 / 2.0,
		y = SCREEN_HEIGHT / GUI_SCALE / 2.0 - 166 / 2.0
	}

	gui:player_inventory {
		name = "inventory",
		pos = gui_pos,
	}

	gui:show(client)
end
