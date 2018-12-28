registry:register_block({
	name = "default:air",
	label = "Air",
	texture = 0,
})

registry:register_block({
	name = "default:dirt",
	label = "Dirt",
	texture = 37,
})

registry:register_block({
	name = "default:cobblestone",
	label = "Cobblestone",
	texture = 38,
	hardness = 2,
	harvest_requirements = 1,
})

registry:register_block({
	name = "default:grass",
	label = "Grass",
	texture = 226,
	item_drop = {
		amount = 1
	}
})

registry:register_block({
	name = "default:leaves",
	label = "Leaves",
	texture = 266,
	hardness = 0.5,
})

registry:register_block({
	name = "default:wood",
	label = "Wood",
	texture = 277,
	hardness = 2
})

registry:register_block({
	name = "default:stone",
	label = "Stone",
	texture = 402,
	hardness = 1.5,
	harvest_requirements = 1,
	item_drop = {
		amount = 1
	},
})

registry:register_block({
	name = "default:sand",
	label = "Sand",
	texture = 369,
})

registry:register_block({
	name = "default:water",
	label = "Water",
	texture = 457,
})

registry:register_block({
	name = "default:glass",
	label = "Glass",
	texture = 168,
})

registry:register_block({
	name = "default:ore_coal",
	label = "Coal Ore",
	texture = 36,
	hardness = 3,
	harvest_requirements = 1,
	item_drop = {
		amount = 1
	},
})

registry:register_block({
	name = "default:planks",
	label = "Planks",
	texture = 316,
})

registry:register_block({
	name = "default:glowstone",
	label = "Glowstone",
	texture = 218,
})

registry:register_block({
	name = "default:workbench",
	label = "Workbench",
	texture = 77,
})

registry:register_block({
	name = "default:furnace",
	label = "Furnace",
	texture = 164,
})

registry:register_block({
	name = "default:ore_iron",
	label = "Iron Ore",
	texture = 254,
	hardness = 3,
	harvest_requirements = 1,
})

registry:register_block({
	name = "default:slab_planks",
	label = "Plank Slab",
	texture = 316,
})

for i = 17, 31 do
	registry:register_block({
		name = "default:undefined" .. i,
		label = "Undefined",
		texture = 4,
	})
end

