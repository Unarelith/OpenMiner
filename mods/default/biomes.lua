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
  id = "default:grassland",
  name = "Grassland",

  params = {
	temperature = -0.6,
	precipitation = 0.4
  },

  top_block = "default:grass",
  ground_block = "default:dirt",
  beach_block = "default:sand",
  liquid_block = "default:water",
  
  flora = {
    {
      block = "default:tallgrass",
      spawns_on = "default:grass",
      probability = 0.25
    },
    {
      block = "default:flower",
      spawns_on = "default:grass",
      probability = 0.1
    }
  }
}

mod:biome {
  id = "default:mountain",
  name = "Mountain",

  params = {
	temperature = 0.5,
	precipitation = 0.3
  },

  top_block = "default:stone",
  ground_block = "default:stone",
  beach_block = "default:stone",
  liquid_block = "default:water",
  
  flora = { }
}

mod:biome {
  id = "default:barren",
  name = "Barren",

  params = {
	temperature = 0.3,
	precipitation = -0.7
  },

  top_block = "default:dirt",
  ground_block = "default:dirt",
  beach_block = "default:dirt",
  liquid_block = "default:water",
  
  flora = { }
}