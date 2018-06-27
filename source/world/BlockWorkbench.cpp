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
#include "InventoryState.hpp"
#include "Player.hpp"

BlockWorkbench::BlockWorkbench() : Block(BlockType::Workbench, 77) {
}

bool BlockWorkbench::onBlockActivated(const glm::ivec3 &, Player &player, World &) const {
	ApplicationStateStack::getInstance().push<InventoryState>(player.inventory(), player.hotbarInventory(), &ApplicationStateStack::getInstance().top());
	return true;
}

