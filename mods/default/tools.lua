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

function register_tool(name, material, mining_speed, harvest_capability)
	local tool_def = {
		id = material .. "_" .. name,
		name = material:gsub("^%l", string.upper) .. " " .. name:gsub("^%l", string.upper),
		tiles = material .. "_" .. name .. ".png",
		max_stack_size = 1,
	}

	if name ~= "sword" then
		if mining_speed then
			tool_def.mining_speed = mining_speed
		end

		if harvest_capability then
			tool_def.harvest_capability = harvest_capability
		end
	end

	if name == "hoe" then
		tool_def.on_item_activated = function(pos, block, player, world, client, server)
			if block:string_id() == "default:grass" or block:string_id() == "default:dirt" then
				world:set_block_from_str(pos.x, pos.y, pos.z, "default: farmland")
			end
		end
	elseif name == "shovel" then
		tool_def.effective_on = {
			"group:om_material_dirt",
			"group:om_material_sand"
		}

		tool_def.on_item_activated = function(pos, block, player, world, client, server)
			if block:string_id() == "default:grass" then
				world:set_block_from_str(pos.x, pos.y, pos.z, "default:grass_path")
			end
		end
	elseif name == "axe" then
		tool_def.effective_on = {
			"group:om_material_wood"
		}
	elseif name == "pickaxe" then
		tool_def.effective_on = {
			"group:om_material_stone"
		}
	end

	mod:item(tool_def)
end

local tools = {axe = 4, hoe = 0, pickaxe = 1, shovel = 2, sword = 0}
local materials = {"wooden", "stone", "iron", "golden", "diamond"}

for i, m in ipairs(materials) do
	for k, v in pairs(tools) do
		register_tool(k, m, i * 2, v)
	end
end

