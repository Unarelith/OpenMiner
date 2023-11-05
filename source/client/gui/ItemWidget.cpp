/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "ItemWidget.hpp"
#include "Registry.hpp"
#include "ResourceHandler.hpp"
#include "TextureAtlas.hpp"

ItemWidget::ItemWidget(Inventory &inventory, u16 x, u16 y, Widget *parent)
	: Widget(18, 18, parent), m_inventory(inventory), m_x(x), m_y(y),
	m_textureAtlas(ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks"))
{
	m_cube.setPosition(9, 9, 0);
	// m_cube.setPosition(8.5, 14, 0);
	// m_cube.setRotation(-172, glm::vec3{0.42, -0.2, 1});
}

void ItemWidget::changeItem(u16 x, u16 y) {
	m_x = x;
	m_y = y;

	m_isEnabled = true;
}

void ItemWidget::disable() {
	m_isEnabled = false;
}

void ItemWidget::update() {
	if (!m_isEnabled) return;

	if (m_itemID != stack().item().id()) {
		if (stack().item().isBlock()) {
			const Block &block = Registry::getInstance().getBlock(stack().item().id());
			const BlockState &blockState = block.getState(0); // FIXME: Get state from item stack
			if (blockState.drawType() != BlockDrawType::XShape && blockState.inventoryImage().empty()) {
				m_cube.updateVertexBuffer(block);
				m_isImage = false;
			}
			else
				updateImage(&blockState);
		}
		else
			updateImage();

		m_itemID = stack().item().id();
	}

	if (m_itemAmount != stack().amount()) {
		m_text.setString(std::to_string(stack().amount()));
		m_text.setPosition(16.f - 4.f - 6.f * floorf(log10f(stack().amount())), 16.f - 6.f, 0);

		m_itemAmount = stack().amount();
	}
}

void ItemWidget::updateImage(const BlockState *blockState) {
	if (m_image.width() == 0 || m_oldTexturePack != Config::texturePack) {
		m_oldTexturePack = Config::texturePack;

		m_image.load(m_textureAtlas.texture());
		m_image.setPosition(1, 1, 0);
		m_image.setClipRect(0, 0, 0, 0);
	}

	FloatRect clipRect = m_textureAtlas.getTexCoords(stack().item().tiles().getTextureForFace(0), false);
	m_image.setClipRect(clipRect.x, clipRect.y, (u16)clipRect.sizeX, (u16)clipRect.sizeY);
	m_image.setScale(16.0f / clipRect.sizeX, 16.0f / clipRect.sizeY);

	if (blockState)
		m_image.setColor(blockState->colorMultiplier());
	else
		m_image.setColor(Color::White);

	m_isImage = true;
}

void ItemWidget::setStack(const std::string &name, u16 amount) {
	m_inventory.setStack(m_x, m_y, name, amount);
	update();
}

void ItemWidget::draw(RenderTarget &target, RenderStates states) const {
	if (!m_isEnabled || stack().amount() == 0) return;

	states.transform *= getTransform();

	if (stack().item().id()) {
		if (!m_isImage)
			target.draw(m_cube, states);
		else
			target.draw(m_image, states);
	}

	if (stack().amount() != 1)
		target.draw(m_text, states);
}

