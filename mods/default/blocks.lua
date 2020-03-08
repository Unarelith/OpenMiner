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
	id = "dirt",
	name = "Dirt",
	tiles = "dirt.png"
}

mod:block {
	id = "cobblestone",
	name = "Cobblestone",
	tiles = "cobblestone.png",
	hardness = 2,
	harvest_requirements = 1,
}

mod:block {
	id = "grass",
	name = "Grass",
	tiles = {"grass_block_top.png", "dirt.png", "grass_block_side.png"},
	color_multiplier = {129, 191, 91, 255},

	item_drop = {
		id = mod:id()..":dirt",
		amount = 1
	}
}

mod:block {
	id = "oak_leaves",
	name = "Oak Leaves",
	tiles = "oak_leaves.png",
	color_multiplier = {106, 173, 51, 255},
	hardness = 0.5,
	draw_type = "leaves",
	-- is_opaque = false, -- FIXME
}

mod:block {
	id = "oak_wood",
	name = "Oak Wood",
	tiles = {"oak_log_top.png", "oak_log.png"},
	hardness = 2
}

mod:block {
	id = "stone",
	name = "Stone",
	tiles = "stone.png",
	hardness = 1.5,
	harvest_requirements = 1,
	item_drop = {
		id = mod:id()..":cobblestone",
		amount = 1
	},
}

mod:block {
	id = "sand",
	name = "Sand",
	tiles = "sand.png"
}

mod:block {
	id = "water",
	name = "Water",
	tiles = "water.png",
	color_multiplier = {51, 115, 255, 217},
	draw_type = "liquid",
	is_opaque = false,
}

mod:block {
	id = "glass",
	name = "Glass",
	tiles = "glass.png",
	draw_type = "glass",
	is_opaque = false,
}

mod:block {
	id = "coal_ore",
	name = "Coal Ore",
	tiles = "coal_ore.png",
	hardness = 3,
	harvest_requirements = 1,
	item_drop = {
		id = mod:id()..":coal",
		amount = 1
	},
}

mod:block {
	id = "oak_planks",
	name = "Oak Wood Planks",
	tiles = "oak_planks.png"
}

mod:block {
	id = "glowstone",
	name = "Glowstone",
	tiles = "glowstone.png",
	is_light_source = true
}

dofile("mods/default/workbench.lua")
dofile("mods/default/furnace.lua")

mod:block {
	id = "iron_ore",
	name = "Iron Ore",
	tiles = "iron_ore.png",
	hardness = 3,
	harvest_requirements = 1,
}

mod:block {
	id = "dandelion",
	name = "Dandelion",
	tiles = "dandelion.png",
	hardness = 0.05,
	draw_type = "xshape",
	bounding_box = {0.25, 0.25, 0.0, 0.5, 0.5, 0.5},
}

mod:block {
	id = "tallgrass",
	name = "Grass",
	tiles = "grass.png",
	color_multiplier = {129, 191, 91, 255},
	hardness = 0.05,
	draw_type = "xshape",
}

mod:block {
	id = "stone_bricks",
	name = "Stone Bricks",
	tiles = "stone_bricks.png",
	hardness = 2,
	harvest_requirements = 1,
}

mod:block {
	id = "bricks",
	name = "Bricks",
	tiles = "bricks.png",
	hardness = 2,
	harvest_requirements = 1,
}

mod:block {
	id = "clay",
	name = "Clay",
	tiles = "clay.png",
	item_drop = {
		id = mod:id()..":clay_ball",
		amount = 4
	}
}

mod:block {
	id = "oak_slab",
	name = "Oak Wood Slab",
	tiles = "oak_planks.png",

	draw_type = "boundingbox",
	is_opaque = false,

	bounding_box = {0, 0, 0, 1, 1, 0.5}
}

mod:block {
	id = "portal",
	name = "Portal",
	tiles = "portal.png",

	draw_type = "glass",
	is_opaque = false,

	on_block_activated = function(pos, player, world, client, server, screen_width, screen_height, gui_scale)
		local dim = (player:dimension() + 1) % 2
		local pos = {x = 0, y = 0, z = 20}

		server:send_player_change_dimension(client.id, pos.x, pos.y, pos.z, dim, client)
		player:set_dimension(dim)
		player:set_position(pos.x, pos.y, pos.z)
	end,
}

mod:block {
	id = "netherrack",
	name = "Netherrack",
	tiles = "netherrack.png",
}

mod:block {
	id = "soul_sand",
	name = "Soul Sand",
	tiles = "soul_sand.png",
}

mod:block {
	id = "lava",
	name = "Lava",
	tiles = "lava.png",

	draw_type = "liquid",
	is_light_source = true,
	is_opaque = false,
}

mod:block {
	id = "cactus",
	name = "Cactus",
	tiles = {"cactus_top.png", "cactus_side.png"},

	draw_type = "boundingbox",
	bounding_box = {2/16, 2/16, 0, 12/16, 12/16, 1}
}

mod:block {
	id = "deadbush",
	name = "Dead Bush",
	tiles = "deadbush.png",
	draw_type = "xshape",
}

