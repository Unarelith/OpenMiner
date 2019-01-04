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
		print("hello")
	end,
}

mod:block {
	id = "furnace",
	name = "Furnace",
	texture = 164,
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

for i = 17, 31 do
	mod:block {
		id = "undefined" .. i,
		name = "Undefined",
		texture = 4,
	}
end

