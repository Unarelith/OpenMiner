registry:register_block({
	texture = 0,
	label = "Air",
})

registry:register_block({
	texture = 37,
	label = "Dirt",
})

registry:register_block({
	texture = 38,
	label = "Cobblestone",
	hardness = 2,
	harvest_requirements = 1,
})

registry:register_block({
	texture = 226,
	label = "Grass",
	item_drop = {
		amount = 1
	}
})

registry:register_block({
	texture = 266,
	label = "Leaves",
	hardness = 0.5,
})

registry:register_block({
	texture = 277,
	label = "Wood",
	hardness = 2
})

registry:register_block({
	texture = 402,
	label = "Stone",
	hardness = 1.5,
	harvest_requirements = 1,
	item_drop = {
		amount = 1
	},
})

registry:register_block({
	texture = 369,
	label = "Sand"
})

registry:register_block({
	texture = 457,
	label = "Water",
})

registry:register_block({
	texture = 168,
	label = "Glass",
})

registry:register_block({
	texture = 36,
	label = "Coal Ore",
	hardness = 3,
	harvest_requirements = 1,
	item_drop = {
		amount = 1
	},
})

registry:register_block({
	texture = 316,
	label = "Planks",
})

registry:register_block({
	texture = 218,
	label = "Glowstone",
})

registry:register_block({
	texture = 77,
	label = "Workbench",
})

registry:register_block({
	texture = 164,
	label = "Furnace",
})

registry:register_block({
	texture = 254,
	label = "Iron Ore",
	hardness = 3,
	harvest_requirements = 1,
})

registry:register_block({
	texture = 316,
	label = "Plank Slab"
})

for i = 17, 31 do
	registry:register_block({
		texture = 4,
		label = "Undefined"
	})
end

registry:register_item({
	label = "Stick",
	texture = 324,
})

registry:register_item({
	label = "Stone Axe",
	texture = 325,
	mining_speed = 4,
	harvest_capability = 4,
})

registry:register_item({
	label = "Stone Hoe",
	texture = 326,
})

registry:register_item({
	label = "Stone Pickaxe",
	texture = 327,
	mining_speed = 4,
	harvest_capability = 1,
})

registry:register_item({
	label = "Stone Shovel",
	texture = 328,
	mining_speed = 4,
	harvest_capability = 2,
})

registry:register_item({
	label = "Stone Sword",
	texture = 329,
})

registry:register_item({
	label = "Coal",
	texture = 111,
	is_fuel = true,
	burn_time = 1600,
})

registry:register_item({
	label = "Iron Ingot",
	texture = 232,
})

registry:register_item({
	label = "Charcoal",
	texture = 41,
	is_fuel = true,
	burn_time = 1600,
})

registry:register_item({
	label = "Wooden Axe",
	texture = 337,
	mining_speed = 2,
	harvest_capability = 4,
})

registry:register_item({
	label = "Wooden Hoe",
	texture = 338,
})

registry:register_item({
	label = "Wooden Pickaxe",
	texture = 339,
	mining_speed = 2,
	harvest_capability = 1,
})

registry:register_item({
	label = "Wooden Shovel",
	texture = 340,
	mining_speed = 2,
	harvest_capability = 2,
})

registry:register_item({
	label = "Wooden Sword",
	texture = 341,
})

