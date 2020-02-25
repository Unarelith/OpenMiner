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
	bounding_box = {0.25, 0.25, 0.0, 0.5, 0.5, 0.5},
}

mod:block {
	id = "tallgrass",
	name = "Tall Grass",
	tiles = "grass.png",
	color_multiplier = {129, 191, 91, 255},
	hardness = 0.05,
	draw_type = 1, -- FIXME: Use string instead
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
	id = "terracotta",
	name = "Terracotta",
	tiles = "terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "white_terracotta",
	name = "White Terracotta",
	tiles = "white_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "orange_terracotta",
	name = "Orange Terracotta",
	tiles = "orange_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "magenta_terracotta",
	name = "Magenta Terracotta",
	tiles = "magenta_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "light_blue_terracotta",
	name = "Light Blue Terracotta",
	tiles = "light_blue_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "yellow_terracotta",
	name = "Yellow Terracotta",
	tiles = "yellow_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "lime_terracotta",
	name = "Lime Terracotta",
	tiles = "lime_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "pink_terracotta",
	name = "Pink Terracotta",
	tiles = "pink_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "gray_terracotta",
	name = "Gray Terracotta",
	tiles = "gray_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "light_gray_terracotta",
	name = "Light Gray Terracotta",
	tiles = "light_gray_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "cyan_terracotta",
	name = "Cyan Terracotta",
	tiles = "cyan_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "purple_terracotta",
	name = "Purple Terracotta",
	tiles = "purple_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "blue_terracotta",
	name = "Blue Terracotta",
	tiles = "blue_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "brown_terracotta",
	name = "Brown Terracotta",
	tiles = "brown_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "green_terracotta",
	name = "Green Terracotta",
	tiles = "green_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "red_terracotta",
	name = "Red Terracotta",
	tiles = "red_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "black_terracotta",
	name = "Black Terracotta",
	tiles = "black_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "white_glazed_terracotta",
	name = "White Glazed Terracotta",
	tiles = "white_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "orange_glazed_terracotta",
	name = "Orange Glazed Terracotta",
	tiles = "orange_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "magenta_glazed_terracotta",
	name = "Magenta Glazed Terracotta",
	tiles = "magenta_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "light_blue_glazed_terracotta",
	name = "Light Blue Glazed Terracotta",
	tiles = "light_blue_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "yellow_glazed_terracotta",
	name = "Yellow Glazed Terracotta",
	tiles = "yellow_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "lime_glazed_terracotta",
	name = "Lime Glazed Terracotta",
	tiles = "lime_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "pink_glazed_terracotta",
	name = "Pink Glazed Terracotta",
	tiles = "pink_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "gray_glazed_terracotta",
	name = "Gray Glazed Terracotta",
	tiles = "gray_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "light_gray_glazed_terracotta",
	name = "Light Gray Glazed Terracotta",
	tiles = "light_gray_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "cyan_glazed_terracotta",
	name = "Cyan Glazed Terracotta",
	tiles = "cyan_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "purple_glazed_terracotta",
	name = "Purple Glazed Terracotta",
	tiles = "purple_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "blue_glazed_terracotta",
	name = "Blue Glazed Terracotta",
	tiles = "blue_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "brown_glazed_terracotta",
	name = "Brown Glazed Terracotta",
	tiles = "brown_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "green_glazed_terracotta",
	name = "Green Glazed Terracotta",
	tiles = "green_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "red_glazed_terracotta",
	name = "Red Glazed Terracotta",
	tiles = "red_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}

mod:block {
	id = "black_glazed_terracotta",
	name = "Black Glazed Terracotta",
	tiles = "black_glazed_terracotta.png",
	hardness = 1.5,
	harvest_requirements = 1
}
