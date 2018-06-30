/*
 * =====================================================================================
 *
 *       Filename:  Registry.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:43:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ItemBlock.hpp"
#include "Registry.hpp"

#include "BlockFurnace.hpp"
#include "BlockWater.hpp"
#include "BlockWorkbench.hpp"
#include "XMLFile.hpp"

Registry *Registry::s_instance = nullptr;

void Registry::registerBlocks() {
	XMLFile doc("resources/config/blocks.xml");

	tinyxml2::XMLElement *blockElement = doc.FirstChildElement("blocks").FirstChildElement("block").ToElement();
	while (blockElement) {
		u16 id = blockElement->UnsignedAttribute("id");
		// const char *name = blockElement->Attribute("name");

		if (id == BlockType::Workbench)    registerBlock<BlockWorkbench>();
		else if (id == BlockType::Furnace) registerBlock<BlockFurnace>();
		else if (id == BlockType::Water)   registerBlock<BlockWater>();
		else {
			u16 textureID = blockElement->UnsignedAttribute("textureID");

			auto &block = registerBlock<Block>(id, textureID);

			float hardness = 0;
			if (blockElement->QueryFloatAttribute("hardness", &hardness) == tinyxml2::XMLError::XML_SUCCESS)
				block.setHardness(hardness);

			unsigned int harvestRequirements = 0;
			if (blockElement->QueryUnsignedAttribute("harvestRequirements", &harvestRequirements) == tinyxml2::XMLError::XML_SUCCESS)
				block.setHarvestRequirements(harvestRequirements);
		}

		blockElement = blockElement->NextSiblingElement("block");
	}
}

void Registry::registerItems() {
	XMLFile doc("resources/config/items.xml");

	tinyxml2::XMLElement *itemElement = doc.FirstChildElement("items").FirstChildElement("item").ToElement();
	while (itemElement) {
		u16 id = itemElement->UnsignedAttribute("id");
		const char *name = itemElement->Attribute("name");

		unsigned int textureID;
		if (itemElement->QueryUnsignedAttribute("textureID", &textureID) == tinyxml2::XMLError::XML_SUCCESS) {
			auto &item = registerItem<Item>(id, textureID, name);

			float miningSpeed = 0;
			if (itemElement->QueryFloatAttribute("miningSpeed", &miningSpeed) == tinyxml2::XMLError::XML_SUCCESS)
				item.setMiningSpeed(miningSpeed);

			unsigned int harvestCapability = 0;
			if (itemElement->QueryUnsignedAttribute("harvestCapability", &harvestCapability) == tinyxml2::XMLError::XML_SUCCESS)
				item.setHarvestCapability(harvestCapability);
		}
		else {
			registerItem<ItemBlock>(id, id, name);
		}

		itemElement = itemElement->NextSiblingElement("item");
	}
}

void Registry::registerRecipes() {
	XMLFile doc("resources/config/recipes.xml");

	tinyxml2::XMLElement *recipeElement = doc.FirstChildElement("recipes").FirstChildElement("recipe").ToElement();
	while (recipeElement) {
		std::vector<std::string> pattern;
		std::map<char, std::vector<u32>> keys;
		ItemStack result;
		bool isShapeless = false;

		tinyxml2::XMLElement *patternElement = recipeElement->FirstChildElement("pattern");
		while (patternElement) {
			pattern.emplace_back(patternElement->Attribute("string"));
			patternElement = patternElement->NextSiblingElement("pattern");
		}

		tinyxml2::XMLElement *keyElement = recipeElement->FirstChildElement("key");
		while (keyElement) {
			char ch = keyElement->Attribute("char")[0];
			u32 item = keyElement->UnsignedAttribute("item");

			std::vector<u32> items;
			items.emplace_back(item);
			keys.emplace(ch, items);

			keyElement = keyElement->NextSiblingElement("key");
		}

		tinyxml2::XMLElement *resultElement = recipeElement->FirstChildElement("result");
		if (resultElement) {
			u16 item = resultElement->UnsignedAttribute("item");
			u16 amount = resultElement->UnsignedAttribute("amount");
			result = ItemStack{item, amount};
		}

		m_recipes.emplace_back(pattern, keys, result, isShapeless);

		recipeElement = recipeElement->NextSiblingElement("recipe");
	}
}

const CraftingRecipe *Registry::getRecipe(const Inventory &inventory) const {
	for (const CraftingRecipe &recipe : m_recipes) {
		if (recipe.isMatching(inventory))
			return &recipe;
	}
	return nullptr;
}

