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
	: Widget(176, 166, parent), m_client(client), m_playerInventory(playerInventory),
	m_blockData(blockData), m_mouseItemWidget(mouseItemWidget),
	m_progressBar("texture-furnace", blockData, ProgressBarType::ItemProcess, this),
	m_burnBar("texture-furnace", blockData, ProgressBarType::BurnProcess, this)
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

	m_progressBar.init(gk::IntRect{176, 14, 24, 17}, gk::Vector2i{80, 35}, "item_progress", 200);
	m_burnBar.init(gk::IntRect{176, 0, 14, 14}, gk::Vector2i{57, 37}, "ticks_remaining", "current_burn_time");
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
	m_progressBar.update();
	m_burnBar.update();

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

	// target.draw(m_burnImage, states);
	// target.draw(m_progressImage, states);

	target.draw(m_progressBar, states);
	target.draw(m_burnBar, states);
}

