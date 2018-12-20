-- Wooden Axe
Registry:registerCraftingRecipe({
	result = {
		item = 41,
		amount = 1
	},

	pattern = {
		"##",
		"#|",
		" |"
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Wooden Hoe
Registry:registerCraftingRecipe({
	result = {
		item = 42,
		amount = 1
	},

	pattern = {
		"##",
		" |",
		" |"
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Wooden Pickaxe
Registry:registerCraftingRecipe({
	result = {
		item = 43,
		amount = 1
	},

	pattern = {
		"###",
		" | ",
		" | "
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Wooden Shovel
Registry:registerCraftingRecipe({
	result = {
		item = 44,
		amount = 1
	},

	pattern = {
		"#",
		"|",
		"|"
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Wooden Sword
Registry:registerCraftingRecipe({
	result = {
		item = 45,
		amount = 1
	},

	pattern = {
		"#",
		"#",
		"|"
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Stone Axe
Registry:registerCraftingRecipe({
	result = {
		item = 33,
		amount = 1
	},

	pattern = {
		"##",
		"#|",
		" |"
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stone Hoe
Registry:registerCraftingRecipe({
	result = {
		item = 34,
		amount = 1
	},

	pattern = {
		"##",
		" |",
		" |"
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stone Pickaxe
Registry:registerCraftingRecipe({
	result = {
		item = 35,
		amount = 1
	},

	pattern = {
		"###",
		" | ",
		" | "
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stone Shovel
Registry:registerCraftingRecipe({
	result = {
		item = 36,
		amount = 1
	},

	pattern = {
		"#",
		"|",
		"|"
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stone Sword
Registry:registerCraftingRecipe({
	result = {
		item = 37,
		amount = 1
	},

	pattern = {
		"#",
		"#",
		"|"
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stick
Registry:registerCraftingRecipe({
	result = {
		item = 32,
		amount = 4
	},

	pattern = {
		'#',
		'#'
	},

	keys = {['#'] = 11}
})

-- Planks
Registry:registerCraftingRecipe({
	result = {
		item = 11,
		amount = 4
	},
	pattern = {"#"},
	keys = {["#"] = 5}
})

-- Workbench
Registry:registerCraftingRecipe({
	result = {
		item = 13,
		amount = 1
	},
	pattern = {
		"##",
		"##"
	},
	keys = {["#"] = 11}
})

-- Furnace
Registry:registerCraftingRecipe({
	result = {
		item = 14,
		amount = 1
	},

	pattern = {
		"###",
		"# #",
		"###"
	},

	keys = {["#"] = 2}
})


-- Plank Slab
Registry:registerCraftingRecipe({
	result = {
		item = 16,
		amount = 6
	},

	pattern = {"###"},

	keys = {['#'] = 11}
})

-- Iron Ingot
Registry:registerSmeltingRecipe({
	input = {item = 15, amount = 1},
	output = {item = 39, amount = 1}
})

-- Stone
Registry:registerSmeltingRecipe({
	input = {item = 2, amount = 1},
	output = {item = 6, amount = 1}
})

-- Glass
Registry:registerSmeltingRecipe({
	input = {item = 7, amount = 1},
	output = {item = 9, amount = 1}
})

-- Charcoal
Registry:registerSmeltingRecipe({
	input = {item = 5, amount = 1},
	output = {item = 40, amount = 1}
})

