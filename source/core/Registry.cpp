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
#include "CraftingRecipe.hpp"
#include "ItemBlock.hpp"
#include "Registry.hpp"
#include "SmeltingRecipe.hpp"

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
		const char *name = blockElement->Attribute("name");

		if (id == BlockType::Workbench)    registerBlock<BlockWorkbench>();
		else if (id == BlockType::Furnace) registerBlock<BlockFurnace>();
		else if (id == BlockType::Water)   registerBlock<BlockWater>();
		else {
			u16 textureID = blockElement->UnsignedAttribute("textureID");

			auto &block = registerBlock<Block>(id, textureID, name);

			float hardness = 0;
			if (blockElement->QueryFloatAttribute("hardness", &hardness) == tinyxml2::XMLError::XML_SUCCESS)
				block.setHardness(hardness);

			unsigned int harvestRequirements = 0;
			if (blockElement->QueryUnsignedAttribute("harvestRequirements", &harvestRequirements) == tinyxml2::XMLError::XML_SUCCESS)
				block.setHarvestRequirements(harvestRequirements);

			tinyxml2::XMLElement *itemDropElement = blockElement->FirstChildElement("item_drop");
			if (itemDropElement) {
				block.setItemDrop(itemDropElement->UnsignedAttribute("id"), itemDropElement->UnsignedAttribute("amount"));
			}

			tinyxml2::XMLElement *boundingBoxElement = blockElement->FirstChildElement("box");
			if (boundingBoxElement) {
				block.setBoundingBox(FloatBox{boundingBoxElement->FloatAttribute("x"),
				                              boundingBoxElement->FloatAttribute("y"),
				                              boundingBoxElement->FloatAttribute("z"),
				                              boundingBoxElement->FloatAttribute("width"),
				                              boundingBoxElement->FloatAttribute("height"),
				                              boundingBoxElement->FloatAttribute("depth")});
			}
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

			unsigned int burnTime = 0;
			if (itemElement->QueryUnsignedAttribute("burnTime", &burnTime) == tinyxml2::XMLError::XML_SUCCESS) {
				item.setIsFuel(true);
				item.setBurnTime(burnTime);
			}
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
		std::string type = recipeElement->Attribute("type");
		if (type == "craft") {
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

			registerRecipe<CraftingRecipe>(pattern, keys, result, isShapeless);
		}
		else if (type == "smelt") {
			tinyxml2::XMLElement *inputElement = recipeElement->FirstChildElement("input");
			u16 inputItem = inputElement->UnsignedAttribute("id");
			u16 inputAmount = inputElement->UnsignedAttribute("amount");
			ItemStack input{inputItem, inputAmount};

			tinyxml2::XMLElement *outputElement = recipeElement->FirstChildElement("output");
			u16 outputItem = outputElement->UnsignedAttribute("id");
			u16 outputAmount = outputElement->UnsignedAttribute("amount");
			ItemStack output{outputItem, outputAmount};

			registerRecipe<SmeltingRecipe>(input, output);
		}

		recipeElement = recipeElement->NextSiblingElement("recipe");
	}
}

const Recipe *Registry::getRecipe(const Inventory &inventory) const {
	for (auto &recipe : m_recipes) {
		if (recipe->isMatching(inventory))
			return recipe.get();
	}
	return nullptr;
}

