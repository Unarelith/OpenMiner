mod:block {
	id = "",
	name = "Air",
	texture = 0,
}

mod:block {
	id = "dirt",
	name = "Dirt",
	texture = 37,
}

mod:block {
	id = "cobblestone",
	name = "Cobblestone",
	texture = 38,
	hardness = 2,
	harvest_requirements = 1,
}

mod:block {
	id = "grass",
	name = "Grass",
	texture = 226,
	item_drop = {
		id = mod:id()..":dirt",
		amount = 1
	}
}

mod:block {
	id = "leaves",
	name = "Leaves",
	texture = 266,
	hardness = 0.5,
}

mod:block {
	id = "wood",
	name = "Wood",
	texture = 277,
	hardness = 2
}

mod:block {
	id = "stone",
	name = "Stone",
	texture = 402,
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
	texture = 369,
}

mod:block {
	id = "water",
	name = "Water",
	texture = 457,
}

mod:block {
	id = "glass",
	name = "Glass",
	texture = 168,
}

mod:block {
	id = "ore_coal",
	name = "Coal Ore",
	texture = 36,
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
	texture = 316,
}

mod:block {
	id = "glowstone",
	name = "Glowstone",
	texture = 218,
}

mod:block {
	id = "workbench",
	name = "Workbench",
	texture = 77,

	on_block_activated = function(pos, player, world)
		-- open_workbench(pos, player, world)
		-- local data = world:getBlockData(position.x, position.y, position.z)
		-- local inventoryState = ApplicationStateStack.getInstance():push<InventoryState>(&gk::ApplicationStateStack::getInstance().top());
		-- inventoryState.setupWidget<WorkbenchWidget>(player.inventory(), data->inventory);
		gui = LuaGUI.new()
		gui:button {
			name = "btn_hello",
			pos = {x = 0, y = 0},
			text = "Hello",
			on_click = function(self)
				print("hello")
			end,
		}
		gui:show()
	end,
}

mod:block {
	id = "furnace",
	name = "Furnace",
	texture = 164,

	on_block_activated = function(pos, player, world)
		open_furnace(pos, player, world)
	end,

	on_tick = function(pos, player, chunk, world)
		update_furnace(pos, player, chunk, world)
	end,
}

mod:block {
	id = "ore_iron",
	name = "Iron Ore",
	texture = 254,
	hardness = 3,
	harvest_requirements = 1,
}

mod:block {
	id = "slab_planks",
	name = "Plank Slab",
	texture = 316,
}

mod:block {
	id = "flower",
	name = "Flower",
	texture = 150,
	hardness = 0.05,
	draw_type = 1,
}

for i = 18, 31 do
	mod:block {
		id = "undefined" .. i,
		name = "Undefined",
		texture = 4,
	}
end

