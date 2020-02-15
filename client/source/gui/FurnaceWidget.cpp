/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "FurnaceWidget.hpp"

FurnaceWidget::FurnaceWidget(ClientCommandHandler &client, MouseItemWidget &mouseItemWidget, Inventory &playerInventory, BlockData &blockData, Widget *parent)
	: Widget(176, 166, parent), m_client(client), m_playerInventory(playerInventory), m_blockData(blockData), m_mouseItemWidget(mouseItemWidget)
{
	m_background.load("texture-furnace");
	m_background.setClipRect(0, 0, 176, 166);

	m_playerInventoryWidget.init(m_playerInventory, 9, 9 * 3);
	m_playerInventoryWidget.setPosition(7, 83, 0);

	m_hotbarInventoryWidget.init(m_playerInventory, 0, 9);
	m_hotbarInventoryWidget.setPosition(7, 141, 0);

	m_inputInventoryWidget.setPosition(55, 16, 0);
	m_outputInventoryWidget.setPosition(115, 34, 0);
	m_fuelInventoryWidget.setPosition(55, 52, 0);

	m_burnImage.setClipRect(176, 0, 14, 14);
	m_burnImage.setPosition(57, 37, 0);

	m_progressImage.setClipRect(176, 14, 24, 17);
	m_progressImage.setPosition(80, 35, 0);
}

void FurnaceWidget::onEvent(const SDL_Event &event) {
	m_playerInventoryWidget.onMouseEvent(event, m_mouseItemWidget);
	m_hotbarInventoryWidget.onMouseEvent(event, m_mouseItemWidget);

	m_inputInventoryWidget.onMouseEvent(event, m_mouseItemWidget);
	m_outputInventoryWidget.onMouseEvent(event, m_mouseItemWidget);

	if (!m_mouseItemWidget.stack().item().id() || m_mouseItemWidget.stack().item().isFuel())
		m_fuelInventoryWidget.onMouseEvent(event, m_mouseItemWidget);
}

void FurnaceWidget::update() {
	u16 ticksRemaining = m_blockData.meta.get<int>("ticks_remaining");
	u16 currentBurnTime = m_blockData.meta.get<int>("current_burn_time");
	u16 itemProgress = m_blockData.meta.get<int>("item_progress");

	if (currentBurnTime) {
		m_burnImage.setPosition(57, 37 + 14 - ticksRemaining * 14 / currentBurnTime, 0);
		m_burnImage.setClipRect(176, 14 - ticksRemaining * 14 / currentBurnTime, 14, ticksRemaining * 14 / currentBurnTime);
	}
	else {
		m_burnImage.setClipRect(0, 0, 0, 0);
	}

	m_progressImage.setClipRect(176, 14, itemProgress * 24 / 200, 17);

	m_inputInventoryWidget.init(m_blockData.inventory, 0, 1);
	m_outputInventoryWidget.init(m_blockData.inventory, 1, 1);
	m_fuelInventoryWidget.init(m_blockData.inventory, 2, 1);

	const ItemWidget *currentItemWidget = nullptr;
	if ((currentItemWidget = m_playerInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_hotbarInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_inputInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_outputInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_fuelInventoryWidget.currentItemWidget()))
		m_mouseItemWidget.updateCurrentItem(currentItemWidget);
	else
		m_mouseItemWidget.updateCurrentItem(nullptr);
}

void FurnaceWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_background, states);

	target.draw(m_playerInventoryWidget, states);
	target.draw(m_hotbarInventoryWidget, states);

	target.draw(m_inputInventoryWidget, states);
	target.draw(m_outputInventoryWidget, states);
	target.draw(m_fuelInventoryWidget, states);

	target.draw(m_burnImage, states);
	target.draw(m_progressImage, states);
}

