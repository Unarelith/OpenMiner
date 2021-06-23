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
#include "SliderWidget.hpp"

SliderWidget::SliderWidget(Widget *parent) : SliderWidget(200, parent) {
}

SliderWidget::SliderWidget(u16 width, Widget *parent) : Widget(width, 20, parent) {
	m_text.setColor(m_defaultTextColor);
	m_text.setShadowColor(gk::Color::fromRGBA32(56, 56, 56));

	m_slider.setClipRect(0, 66, 8, 20);
	m_background.setClipRect(0, 46, width, 20);

	m_sliderBorder.setClipRect(200 - 2, 66, 2, 20);
	m_backgroundBorder.setClipRect(200 - 2, 46, 2, 20);

	m_sliderBorder.setPosition(8 - 2, 0);
	m_backgroundBorder.setPosition(width - 2.f, 0);
}

void SliderWidget::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		if (m_isDragging || isPointInWidget(event.motion.x, event.motion.y)) {
			m_text.setColor(m_hoverColor);

			updatePercentage(event.motion.x);
		}
		else
			m_text.setColor(m_defaultTextColor);

		if (m_isDragging)
			m_callback(*this, event.type);
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		m_isDragging = isPointInWidget(event.button.x, event.button.y);
		updatePercentage(event.button.x);
		if (m_isDragging)
			m_callback(*this, event.type);
	}
	else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
		if (m_isDragging)
			m_callback(*this, event.type);
		setCurrentValue(getCurrentValue());
		m_isDragging = false;
	}
}

void SliderWidget::setCurrentValue(int currentValue) {
	m_percentage = float(currentValue - m_min) / float(m_max - m_min);

	updateSliderPosition();
}

void SliderWidget::setText(const std::string &text) {
	m_text.setString(text);
	m_text.updateVertexBuffer();
	m_text.setPosition(
		(float)(m_width / 2 - m_text.getSize().x / 2),
		(float)(m_height / 2 - m_text.getSize().y / 2)
	);
}

void SliderWidget::updatePercentage(s32 mouseX) {
	if (m_isDragging) {
		m_percentage = ((float)mouseX - getGlobalBounds().x) / Config::guiScale / (float)m_width;
		m_percentage = std::clamp(m_percentage, 0.f, 1.f);

		updateSliderPosition();
	}
}

void SliderWidget::updateSliderPosition() {
	m_slider.setPosition(m_percentage * float(m_width - 8), 0.f);
	m_sliderBorder.setPosition(m_percentage * float(m_width - 8) + 6.f, 0.f);
}

void SliderWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_background, states);
	target.draw(m_backgroundBorder, states);

	target.draw(m_slider, states);
	target.draw(m_sliderBorder, states);

	target.draw(m_text, states);
}

