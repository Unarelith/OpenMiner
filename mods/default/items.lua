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

mod:item {
	id = "stick",
	name = "Stick",
	tiles = "stick.png"
}

mod:item {
	id = "stone_axe",
	name = "Stone Axe",
	tiles = "stone_axe.png",
	mining_speed = 4,
	harvest_capability = 4,
}

mod:item {
	id = "stone_hoe",
	name = "Stone Hoe",
	tiles = "stone_hoe.png",

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:farmland")
		end
	end
}

mod:item {
	id = "stone_pickaxe",
	name = "Stone Pickaxe",
	tiles = "stone_pickaxe.png",
	mining_speed = 4,
	harvest_capability = 1,
}

mod:item {
	id = "stone_shovel",
	name = "Stone Shovel",
	tiles = "stone_shovel.png",
	mining_speed = 4,
	harvest_capability = 2,

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:grass_path")
		end
	end
}

mod:item {
	id = "stone_sword",
	name = "Stone Sword",
	tiles = "stone_sword.png"
}

mod:item {
	id = "coal",
	name = "Coal",
	tiles = "coal.png",

	groups = {
		om_fuel = 1600,
	},
}

mod:item {
	id = "iron_ingot",
	name = "Iron Ingot",
	tiles = "iron_ingot.png"
}

mod:item {
	id = "charcoal",
	name = "Charcoal",
	tiles = "charcoal.png",

	groups = {
		om_fuel = 1600,
	},
}

mod:item {
	id = "wooden_axe",
	name = "Wooden Axe",
	tiles = "wooden_axe.png",
	mining_speed = 2,
	harvest_capability = 4,
}

mod:item {
	id = "wooden_hoe",
	name = "Wooden Hoe",
	tiles = "wooden_hoe.png",

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:farmland")
		end
	end
}

mod:item {
	id = "wooden_pickaxe",
	name = "Wooden Pickaxe",
	tiles = "wooden_pickaxe.png",
	mining_speed = 2,
	harvest_capability = 1,
}

mod:item {
	id = "wooden_shovel",
	name = "Wooden Shovel",
	tiles = "wooden_shovel.png",
	mining_speed = 2,
	harvest_capability = 2,

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:grass_path")
		end
	end
}

mod:item {
	id = "wooden_sword",
	name = "Wooden Sword",
	tiles = "wooden_sword.png",
}

mod:item {
	id = "brick",
	name = "Brick",
	tiles = "brick.png"
}

mod:item {
	id = "clay_ball",
	name = "Clay",
	tiles = "clay_ball.png"
}

mod:item {
	id = "diamond",
	name = "Diamond",
	tiles = "diamond.png"
}

mod:item {
	id = "gold_ingot",
	name = "Gold Ingot",
	tiles = "gold_ingot.png"
}

mod:item {
	id = "iron_pickaxe",
	name = "Iron Pickaxe",
	tiles = "iron_pickaxe.png",
	mining_speed = 6,
	harvest_capability = 1
}

mod:item {
	id = "iron_sword",
	name = "Iron Sword",
	tiles = "iron_sword.png"
}

mod:item {
	id = "iron_axe",
	name = "Iron Axe",
	tiles = "iron_axe.png",
	mining_speed = 6,
	harvest_capability = 4
}

mod:item {
	id = "iron_hoe",
	name = "Iron Hoe",
	tiles = "iron_hoe.png",

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:farmland")
		end
	end
}

mod:item {
	id = "iron_shovel",
	name = "Iron Shovel",
	tiles = "iron_shovel.png",
	mining_speed = 6,
	harvest_capability = 2,

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:grass_path")
		end
	end
}

mod:item {
	id = "diamond_pickaxe",
	name = "Diamond Pickaxe",
	tiles = "diamond_pickaxe.png",
	mining_speed = 8,
	harvest_capability = 1
}

mod:item {
	id = "diamond_sword",
	name = "Diamond Sword",
	tiles = "diamond_sword.png"
}

mod:item {
	id = "diamond_axe",
	name = "Diamond Axe",
	tiles = "diamond_axe.png",
	mining_speed = 8,
	harvest_capability = 4
}

mod:item {
	id = "diamond_hoe",
	name = "Diamond Hoe",
	tiles = "diamond_hoe.png",

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:farmland")
		end
	end
}

mod:item {
	id = "diamond_shovel",
	name = "Diamond Shovel",
	tiles = "diamond_shovel.png",
	mining_speed = 8,
	harvest_capability = 2,

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:grass_path")
		end
	end
}

mod:item {
	id = "golden_pickaxe",
	name = "Golden Pickaxe",
	tiles = "golden_pickaxe.png",
	mining_speed = 10,
	harvest_capability = 1
}

mod:item {
	id = "golden_sword",
	name = "Golden Sword",
	tiles = "golden_sword.png"
}

mod:item {
	id = "golden_axe",
	name = "Golden Axe",
	tiles = "golden_axe.png",
	mining_speed = 10,
	harvest_capability = 4
}

mod:item {
	id = "golden_hoe",
	name = "Golden Hoe",
	tiles = "golden_hoe.png",

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:farmland")
		end
	end
}

mod:item {
	id = "golden_shovel",
	name = "Golden Shovel",
	tiles = "golden_shovel.png",
	mining_speed = 10,
	harvest_capability = 2,

	on_item_activated = function(pos, block, player, world, client, server, screen_width, screen_height, gui_scale)
		if block:string_id() == "default:grass" then
			world:set_block_from_str(pos.x, pos.y, pos.z, "default:grass_path")
		end
	end
}

mod:item {
	id = "wheat",
	name = "Wheat",
	tiles = "wheat.png",
}

mod:item {
	id = "bread",
	name = "Bread",
	tiles = "bread.png",
}

