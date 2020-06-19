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

mod:entity {
	id = "item_drop",

	properties = {
		visual = {
			type = "inventorycube",
			size = 0.25,
			origin = {0.125, 0.125, 0.125},
		},

		is_rotatable = true,

		animation = {
			{
				type = "rotation",
				axis = {0, 0, 1},
				angle = 0.5
			},
			{
				type = "translation",
				delta = {0, 0, -0.0005},
				min = -0.2,
				max = 0,
				loop = true
			}
		},

		hitbox = {0, 0, 0, 0.25, 0.25, 0.25},
	},

	on_collision = function(entity, player, server)
		mods["default"]:give_item_stack(player, entity:item_stack())
		mods["default"]:despawn_entity(entity)
	end,
}

mod:option("use_item_drops", false)

openminer:add_listener(Event.BlockDigged, function(pos, block, player, world, client, server)
	if openminer.get_config("default:use_item_drops")then
		mods["default"]:spawn_entity("default:item_drop", {
			position = {pos.x + 0.5, pos.y + 0.5, pos.z + 0.5},
			dimension = world:dimension():id(),

			item_stack = {block:get_item_drop():item():string_id(), block:get_item_drop():amount()}
		})
	else
		mods["default"]:give_item_stack(player, block:get_item_drop())
	end
end)

