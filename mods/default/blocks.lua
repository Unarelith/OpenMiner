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
	id = "leaves",
	name = "Leaves",
	tiles = "oak_leaves.png",
	color_multiplier = {106, 173, 51, 255},
	hardness = 0.5,
	draw_type = 2, -- FIXME: Use string instead
	-- is_opaque = false, -- FIXME
}

mod:block {
	id = "wood",
	name = "Wood",
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
	draw_type = 3, -- FIXME: Use string instead
	is_opaque = false,
}

mod:block {
	id = "glass",
	name = "Glass",
	tiles = "glass.png",
	draw_type = 4, -- FIXME: Use string instead
	is_opaque = false,
}

mod:block {
	id = "ore_coal",
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
	id = "planks",
	name = "Planks",
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
	id = "flower",
	name = "Flower",
	tiles = "dandelion.png",
	hardness = 0.05,
	draw_type = 1, -- FIXME: Use string instead
	bounding_box = {0.25, 0.0, 0.25, 0.5, 0.5, 0.5},
}

mod:block {
	id = "tallgrass",
	name = "Tall Grass",
	tiles = "grass.png",
	color_multiplier = {129, 191, 91, 255},
	hardness = 0.05,
	draw_type = 1, -- FIXME: Use string instead
}

