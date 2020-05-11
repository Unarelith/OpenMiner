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
#include "Events.hpp"
#include "MenuWidget.hpp"

MenuWidget::MenuWidget(u16 width, u16 height, Widget *parent) : Widget(parent) {
	reset(width, height);
}

void MenuWidget::reset(u16 width, u16 height) {
	m_width = width;
	m_height = height;

	m_buttons.clear();
	m_buttons.reserve(m_width * m_height);
}

void MenuWidget::onEvent(const sf::Event &event) {
	for (std::size_t i = 0 ; i < m_buttons.size() ; ++i) {
		m_buttons.at(i).onEvent(event);

		if (event.type == sf::Event::Resized) {
			int x = i % m_width;
			int y = i / m_width;

			updateButtonPosition(m_buttons.at(i), x, y);
		}
	}
}

void MenuWidget::onGuiScaleChanged(const GuiScaleChangedEvent &event) {
	setScale(event.guiScale, event.guiScale, 1);

	for (std::size_t i = 0 ; i < m_buttons.size() ; ++i) {
		int x = i % m_width;
		int y = i / m_width;

		updateButtonPosition(m_buttons.at(i), x, y);
	}
}

TextButton &MenuWidget::addButton(const std::string &text, const TextButton::CppCallback &callback) {
	int x = m_buttons.size() % m_width;
	int y = m_buttons.size() / m_width;

	m_buttons.emplace_back(this);

	TextButton &button = m_buttons.back();
	button.setText(text);
	button.setCallback(callback);
	updateButtonPosition(button, x, y);
	return button;
}

void MenuWidget::updateButtonPosition(TextButton &button, int x, int y) {
	button.setPosition(Config::screenWidth  / getScale().x / 2.0f - (m_width  * (button.width()  + s_horizontalSpacing) - s_horizontalSpacing) / 2 + x * (button.width() + s_horizontalSpacing),
	                   Config::screenHeight / getScale().y / 2.0f - (m_height * (button.height() + s_verticalSpacing)   - s_verticalSpacing)   / 2 + y * (button.height() + s_verticalSpacing), 0);
}

void MenuWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	for (const TextButton &button : m_buttons) {
		if (!button.text().empty())
			target.draw(button, states);
	}
}

