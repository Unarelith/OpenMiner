/*
 * =====================================================================================
 *
 *       Filename:  BlockWorkbench.cpp
 *
 *    Description:
 *
 *        Created:  27/06/2018 04:53:03
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ApplicationStateStack.hpp"
#include "BlockWorkbench.hpp"
#include "Exception.hpp"
#include "InventoryState.hpp"
#include "Player.hpp"
#include "WorkbenchWidget.hpp"
#include "World.hpp"

BlockWorkbench::BlockWorkbench() : Block(BlockType::Workbench, 77) {
}

bool BlockWorkbench::onBlockActivated(const glm::ivec3 &position, Player &player, World &world) const {
	BlockData *data = world.getBlockData(position.x, position.y, position.z);
	if (!data)
		throw EXCEPTION("BlockWorkbench at (", position.x, position.y, position.z, ") has no inventory");

	auto &inventoryState = ApplicationStateStack::getInstance().push<InventoryState>(&ApplicationStateStack::getInstance().top());
	inventoryState.setupWidget<WorkbenchWidget>(player.inventory(), player.hotbarInventory(), data->inventory);

	return true;
}

