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
#include "ScrollableList.hpp"

u16 ScrollableListElement::widgetWidth = 200;

ScrollableListElement::ScrollableListElement(const std::string &line1, const std::string &line2, const std::string &line3, Widget *parent) : Widget(widgetWidth, 0, parent) {
	m_line1.setString(line1);
	m_line1.updateVertexBuffer();
	m_line1.setPosition(0, 1);
	m_line1.setShadowEnabled(false);

	m_line2.setString(line2);
	m_line2.updateVertexBuffer();
	m_line2.setPosition(0, m_line1.getPosition().y + m_line1.getSize().y + 1);
	m_line2.setColor(gk::Color{128, 128, 128});
	m_line2.setShadowEnabled(false);

	m_line3.setString(line3);
	m_line3.updateVertexBuffer();
	m_line3.setPosition(0, m_line2.getPosition().y + m_line2.getSize().y);
	m_line3.setColor(gk::Color{128, 128, 128});
	m_line3.setShadowEnabled(false);

	m_height = m_line3.getPosition().y + m_line3.getSize().y + 2;

	m_icon.setPosRect(0, 0, m_height, m_height);
	m_line1.move(m_icon.posRect().sizeX + 3, 0);
	m_line2.move(m_icon.posRect().sizeX + 3, 0);
	m_line3.move(m_icon.posRect().sizeX + 3, 0);
}

void ScrollableListElement::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_icon, states);

	target.draw(m_line1, states);
	target.draw(m_line2, states);
	target.draw(m_line3, states);
}

ScrollableList::ScrollableList() : Widget(ScrollableListElement::widgetWidth, 0) {
	m_cursor.setOutlineThickness(1);
	m_cursor.setOutlineColor(gk::Color::White);
	m_cursor.setFillColor(gk::Color::Transparent);
	m_cursor.setSize(m_width, m_height);
}

void ScrollableList::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		for (auto &it : m_elements) {
			if (it.isPointInWidget(event.button.x, event.button.y)) {
				m_cursor.setSize(it.width() + 2, it.height() + 2);
				m_cursor.setPosition(-1, it.getPosition().y - 1);

				m_selectedElement = &it;
			}
		}
	}
}

void ScrollableList::addElement(const std::string &line1, const std::string &line2, const std::string &line3) {
	m_elements.emplace_back(line1, line2, line3, this);
	m_elements.back().setPosition(0, (m_elements.size() - 1) * (m_elements.back().height() + 4) + 2);

	m_height += m_elements.back().height();
}

void ScrollableList::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	for (auto &it : m_elements)
		target.draw(it, states);

	if (m_selectedElement)
		target.draw(m_cursor, states);
}

