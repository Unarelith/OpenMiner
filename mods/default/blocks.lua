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
	tiles = "dirt.png",
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
	hardness = 0,
	draw_type = "leaves",
	-- is_opaque = false, -- FIXME

	item_drop = {},
}

mod:block {
	id = "oak_wood",
	name = "Oak Wood",
	tiles = {"oak_log_top.png", "oak_log.png"},
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
	tiles = "sand.png",
}

mod:block {
	id = "water",
	name = "Water",

	tiles = "water.png",
	color_multiplier = {51, 115, 255, 217},

	draw_type = "liquid",
	is_opaque = false,

	fog_depth = 20.0,
	fog_color = {0, 128, 255},
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
	tiles = "oak_planks.png",

	hardness = 1,

	groups = {
		default_planks = 1
	}
}

mod:block {
	id = "glowstone",
	name = "Glowstone",
	tiles = "glowstone.png",
	is_light_source = true,
}

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
	hardness = 0,
	draw_type = "xshape",
	bounding_box = {0.25, 0.25, 0.0, 0.5, 0.5, 0.5},
}

mod:block {
	id = "tallgrass",
	name = "Grass",
	tiles = "grass.png",
	color_multiplier = {129, 191, 91, 255},
	hardness = 0,
	draw_type = "xshape",
	item_drop = {},
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

	on_block_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		local dim = (player:dimension() + 1) % 2
		local pos = {
			x = math.floor(player:x()),
			y = math.floor(player:y()),
			z = math.floor(player:z())
		}

		server:send_player_change_dimension(client.id, pos.x, pos.y, pos.z, dim, client)
		player:set_dimension(dim)
		-- player:set_position(pos.x, pos.y, pos.z)
	end,
}

mod:block {
	id = "netherrack",
	name = "Netherrack",
	tiles = "netherrack.png",
	harvest_requirements = 1,
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

	fog_depth = 10.0,
	fog_color = {255, 128, 0},
}

mod:block {
	id = "cactus",
	name = "Cactus",
	tiles = {"cactus_top.png", "cactus_bottom.png", "cactus_side.png"},

	draw_type = "cactus",
	bounding_box = {1/16, 1/16, 0, 14/16, 14/16, 1};

	on_block_destroyed = function(pos, world)
		if world:get_block(pos.x, pos.y, pos.z + 1) == world:get_block(pos.x, pos.y, pos.z) then
			world:set_block(pos.x, pos.y, pos.z + 1, 0)
		end
	end,
}

mod:block {
	id = "deadbush",
	name = "Dead Bush",
	tiles = "deadbush.png",
	draw_type = "xshape",
	hardness = 0,
}

mod:block {
	id = "debug",
	name = "Debug",
	tiles = {
		"debug/f.png",
		"debug/g.png",
		"debug/j.png",
		"debug/l.png",
		"debug/p.png",
		"debug/r.png",
	},
}

mod:block {
	id = "obsidian",
	name = "Obsidian",
	tiles = "obsidian.png",

	hardness = 8,
	harvest_requirements = 1,
}

mod:block {
	id = "reeds",
	name = "Sugar Canes",
	tiles = "reeds.png",
	inventory_image = "reeds_item.png",

	draw_type = "xshape",
	hardness = 0,

	on_block_destroyed = function(pos, world)
		if world:get_block(pos.x, pos.y, pos.z + 1) == world:get_block(pos.x, pos.y, pos.z) then
			world:set_block(pos.x, pos.y, pos.z + 1, 0)
		end
	end,
}

mod:block {
	id = "redstone_lamp",
	name = "Redstone Lamp",
	tiles = "redstone_lamp_off.png",
	harvest_requirements = 1,

	states = {
		{ is_light_source = true, tiles = "redstone_lamp_on.png" }
	},

	on_block_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		local next_state = math.abs(world:get_block_state(pos.x, pos.y, pos.z):id() - 1)
		world:set_block_state(pos.x, pos.y, pos.z, next_state)
	end
}

mod:block {
	id = "farmland",
	name = "Farmland",
	tiles = {"farmland_dry.png", "dirt.png", "dirt.png"},
	is_opaque = false,

	states = {
		{ alt_tiles = {"farmland_wet.png", "dirt.png", "dirt.png"} },
	},

	draw_type = "boundingbox",
	bounding_box = {0, 0, 0, 1, 1, 15 / 16},
}

mod:block {
	id = "grass_path",
	name = "Grass Path",
	tiles = {"grass_path_top.png", "dirt.png", "grass_path_side.png"},
	is_opaque = false,

	draw_type = "boundingbox",
	bounding_box = {0, 0, 0, 1, 1, 15 / 16},
}

mod:block {
	id = "seeds",
	name = "Seeds",
	tiles = "wheat_stage_0.png",
	draw_type = "xshape",
	inventory_image = "seeds_wheat.png",
	hardness = 0,

	bounding_box = {0, 0, 0, 1, 1, 1.0 / 16.0},
	draw_offset = {0, 0, -1.0 / 16.0},

	states = {
		{ tiles = "wheat_stage_1.png", bounding_box = {0, 0, 0, 1, 1, 3.0 / 16.0}, },  -- 1
		{ tiles = "wheat_stage_2.png", bounding_box = {0, 0, 0, 1, 1, 5.0 / 16.0}, },  -- 2
		{ tiles = "wheat_stage_3.png", bounding_box = {0, 0, 0, 1, 1, 8.0 / 16.0}, },  -- 3
		{ tiles = "wheat_stage_4.png", bounding_box = {0, 0, 0, 1, 1, 10.0 / 16.0}, }, -- 4
		{ tiles = "wheat_stage_5.png", bounding_box = {0, 0, 0, 1, 1, 12.0 / 16.0}, }, -- 5
		{ tiles = "wheat_stage_6.png", bounding_box = {0, 0, 0, 1, 1, 14.0 / 16.0}, }, -- 6
		{ tiles = "wheat_stage_7.png", bounding_box = {0, 0, 0, 1, 1, 1}, },           -- 7
	},

	tick_randomly = true,
	tick_probability = 0.01,

	on_tick = function(pos, block, chunk, world)
		local block_param = world:get_data(pos.x, pos.y, pos.z)
		local current_state = block:param():get_param(BlockParamType.State, block_param)
		if current_state < 7 then
			world:set_data(pos.x, pos.y, pos.z, current_state + 1)
		end
	end,

	on_block_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		local block_param = world:get_data(pos.x, pos.y, pos.z)
		local current_state = block:param():get_param(BlockParamType.State, block_param)
		if current_state >= 7 then
			world:set_data(pos.x, pos.y, pos.z,
				block:param():set_param(BlockParamType.State, block_param, 0))

			-- FIXME: It should drop the item if 'default:use_item_drops' is enabled
			local item_stack = ItemStack.new("default:wheat", 1)
			mods["default"]:give_item_stack(player, item_stack)
		end
	end
}

dofile("blocks/workbench.lua")
dofile("blocks/furnace.lua")
dofile("blocks/door.lua")

