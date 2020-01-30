/*
 * =====================================================================================
 *
 *       Filename:  ItemWidget.cpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:11:11
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/resource/ResourceHandler.hpp>

#include "ItemWidget.hpp"
#include "Registry.hpp"
#include "TextureAtlas.hpp"

ItemWidget::ItemWidget(Inventory &inventory, u16 x, u16 y, Widget *parent)
	: Widget(18, 18, parent), m_inventory(inventory), m_x(x), m_y(y),
	m_textureAtlas(gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks"))
{
	m_cube.setPosition(-12.7, -14.6, 0);
	// m_cube.setPosition(8.5, 14, 0);
	// m_cube.setRotation(-172, glm::vec3{0.42, -0.2, 1});
}

void ItemWidget::update() {
	if (stack().item().isBlock()) {
		const Block &block = Registry::getInstance().getBlock(stack().item().id());
		if (block.drawType() != BlockDrawType::XShape) {
			m_cube.updateVertexBuffer(block);
			m_isImage = false;
		}
		else
			updateImage();
	}
	else
		updateImage();

	m_text.setText(std::to_string(stack().amount()));
	m_text.setPosition(16 - 4 - 6 * floor(log10(stack().amount())), 16 - 6, 0);
}

void ItemWidget::updateImage() {
	if (m_image.width() == 0) {
		m_image.load(m_textureAtlas.texture());
		m_image.setPosition(1, 1, 0);
		m_image.setClipRect(0, 0, 0, 0);
	}

	gk::FloatRect clipRect = m_textureAtlas.getTexCoords(stack().item().tiles().getTextureForFace(0), false);
	m_image.setClipRect(clipRect.x, clipRect.y, clipRect.width, clipRect.height);
	m_image.setScale(16.0f / clipRect.width, 16.0f / clipRect.height);

	m_isImage = true;
}

void ItemWidget::setStack(const std::string &name, unsigned int amount) {
	m_inventory.setStack(m_x, m_y, name, amount);
	update();
}

void ItemWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
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

