registry:register_block({
	id = 0,
	texture = 0,
	name = "Air",
})

registry:register_block({
	id = 1,
	texture = 37,
	name = "Dirt",
})

registry:register_block({
	id = 2,
	texture = 38,
	name = "Cobblestone",
	hardness = 2,
	harvest_requirements = 1,
})

registry:register_block({
	id = 3,
	texture = 226,
	name = "Grass",
	item_drop = {
		id = 1,
		amount = 1
	}
})

registry:register_block({
	id = 4,
	texture = 266,
	name = "Leaves",
	hardness = 0.5,
})

registry:register_block({
	id = 5,
	texture = 277,
	name = "Wood",
	hardness = 2
})

registry:register_block({
	id = 6,
	texture = 402,
	name = "Stone",
	hardness = 1.5,
	harvest_requirements = 1,
	item_drop = {
		id = 2,
		amount = 1
	},
})

registry:register_block({
	id = 7,
	texture = 369,
	name = "Sand"
})

registry:register_block({
	id = 8,
	texture = 457,
	name = "Water",
})

registry:register_block({
	id = 9,
	texture = 168,
	name = "Glass",
})

registry:register_block({
	id = 10,
	texture = 36,
	name = "Coal Ore",
	hardness = 3,
	harvest_requirements = 1,
	item_drop = {
		id = 38,
		amount = 1
	},
})

registry:register_block({
	id = 11,
	texture = 316,
	name = "Planks",
})

registry:register_block({
	id = 12,
	texture = 218,
	name = "Glowstone",
})

registry:register_block({
	id = 13,
	texture = 77,
	name = "Workbench",
})

registry:register_block({
	id = 14,
	texture = 164,
	name = "Furnace",
})

registry:register_block({
	id = 15,
	texture = 254,
	name = "Iron Ore",
	hardness = 3,
	harvest_requirements = 1,
})

registry:register_block({
	id = 16,
	texture = 316,
	name = "Plank Slab"
})

for i = 17, 31 do
	registry:register_block({
		id = i,
		texture = 4,
		name = "Undefined"
	})
end

registry:register_item({
	id = 32,
	name = "Stick",
	texture = 324,
})

registry:register_item({
	id = 33,
	name = "Stone Axe",
	texture = 325,
	mining_speed = 4,
	harvest_capability = 4,
})

registry:register_item({
	id = 34,
	name = "Stone Hoe",
	texture = 326,
})

registry:register_item({
	id = 35,
	name = "Stone Pickaxe",
	texture = 327,
	mining_speed = 4,
	harvest_capability = 1,
})

registry:register_item({
	id = 36,
	name = "Stone Shovel",
	texture = 328,
	mining_speed = 4,
	harvest_capability = 2,
})

registry:register_item({
	id = 37,
	name = "Stone Sword",
	texture = 329,
})

registry:register_item({
	id = 38,
	name = "Coal",
	texture = 111,
	is_fuel = true,
	burn_time = 1600,
})

registry:register_item({
	id = 39,
	name = "Iron Ingot",
	texture = 232,
})

registry:register_item({
	id = 40,
	name = "Charcoal",
	texture = 41,
	is_fuel = true,
	burn_time = 1600,
})

registry:register_item({
	id = 41,
	name = "Wooden Axe",
	texture = 337,
	mining_speed = 2,
	harvest_capability = 4,
})

registry:register_item({
	id = 42,
	name = "Wooden Hoe",
	texture = 338,
})

registry:register_item({
	id = 43,
	name = "Wooden Pickaxe",
	texture = 339,
	mining_speed = 2,
	harvest_capability = 1,
})

registry:register_item({
	id = 44,
	name = "Wooden Shovel",
	texture = 340,
	mining_speed = 2,
	harvest_capability = 2,
})

registry:register_item({
	id = 45,
	name = "Wooden Sword",
	texture = 341,
})

