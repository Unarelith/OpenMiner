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
	item_drop = {
		id = mod:id()..":dirt",
		amount = 1
	}
}

mod:block {
	id = "leaves",
	name = "Leaves",
	tiles = "oak_leaves.png",
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

