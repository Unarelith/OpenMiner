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

mod:biome {
	id = "grassland",
	name = "Grassland",

	params = {
		temperature = 0.5,
		precipitation = 0.3
	},

	top_block = "default:grass",
	ground_block = "default:dirt",
	deep_block = "default:stone",
	beach_block = "default:sand",
	liquid_block = "default:water",
	
	trees = {
		{
			type = "default:oak",
			probability = 0.01041666667
		}
	},

	-- Currently, all ores are defined per-biome. In a later update,
	-- we could see ores defined mostly by worldtype, and some extras
	-- added per biome. In addition, given the large world depth, we
	-- could decide whether ores at a point very far down should
	-- still depend on the surface biome, or only on the worldtype.
	ores = {
		{
			block = "default:iron_ore",
			probability = 0.0003,
			size = 8
		}
	},
	
	flora = {
		{
			block = "default:tallgrass",
			spawns_on = "default:grass",
			probability = 0.25
		},
		{
			block = "default:dandelion",
			spawns_on = "default:grass",
			probability = 0.1
		}
	}
}

mod:biome {
	id = "mountain",
	name = "Mountain",

	params = {
		temperature = -0.6,
		precipitation = 0.4
	},

	top_block = "default:stone",
	ground_block = "default:stone",
	deep_block = "default:stone",
	beach_block = "default:stone",
	liquid_block = "default:water",
	
	trees = {
		{
			type = "default:oak",
			probability = 0.00390625
		}
	}
}

mod:biome {
	id = "desert",
	name = "Desert",

	params = {
		temperature = -0.3,
		precipitation = -0.7
	},

	top_block = "default:sand",
	ground_block = "default:sand",
	deep_block = "default:stone",
	beach_block = "default:sand",
	liquid_block = "default:water"
}
