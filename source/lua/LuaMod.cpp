/*
 * =====================================================================================
 *
 *       Filename:  LuaMod.cpp
 *
 *    Description:
 *
 *        Created:  04/01/2019 17:20:09
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "LuaMod.hpp"
#include "Registry.hpp"

void LuaMod::registerBlock(const sol::table &table) {
	u32 textureID = table["texture"].get<u32>();
	std::string id = m_id + ":" + table["id"].get<std::string>();
	std::string name = table["name"].get<std::string>();

	Block &block = Registry::getInstance().registerBlock(textureID, id, name);
	block.setHarvestRequirements(table["harvest_requirements"].get_or(0));
	block.setHardness(table["hardness"].get_or(1.0f));

	sol::optional<sol::table> itemDrop = table["item_drop"];
	if (itemDrop != sol::nullopt) {
		std::string dropID = itemDrop.value()["id"];
		u16 dropAmount = itemDrop.value()["amount"];
		block.setItemDrop(dropID, dropAmount);
	}

	Registry::getInstance().registerItem(block.textureID(), id, name).setIsBlock(true);
}

