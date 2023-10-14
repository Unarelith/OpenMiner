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
	m_sliders.clear();

	Widget::m_width = 0;
	Widget::m_height = 0;
}

void MenuWidget::onEvent(const SDL_Event &event) {
	std::size_t sliderCount = m_sliders.size();
	std::size_t buttonCount = m_buttons.size();

	for (auto &it : m_sliders) {
		it.first.onEvent(event);

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			updateWidgetPosition(it.first, it.second.x, it.second.y);
		}

		if (sliderCount != m_sliders.size())
			break;
	}

	for (auto &it : m_buttons) {
		it.first.onEvent(event);

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			updateWidgetPosition(it.first, it.second.x, it.second.y);
		}

		if (buttonCount != m_buttons.size())
			break;
	}
}

void MenuWidget::onGuiScaleChanged(const GuiScaleChangedEvent &event) {
	setScale(event.guiScale, event.guiScale, 1);

	for (auto &it : m_buttons) {
		updateWidgetPosition(it.first, it.second.x, it.second.y);
	}

	for (auto &it : m_sliders) {
		updateWidgetPosition(it.first, it.second.x, it.second.y);
	}
}

TextButton &MenuWidget::addButton(const std::string &text, const TextButton::CppCallback &callback, u16 width) {
	int x = int((m_buttons.size() + m_sliders.size()) % m_width);
	int y = int((m_buttons.size() + m_sliders.size()) / m_width);

	m_buttons.emplace_back(std::piecewise_construct,
		std::forward_as_tuple(width, this), std::forward_as_tuple(x, y));

	TextButton &button = m_buttons.back().first;
	button.setText(text);
	button.setCallback(callback);

	updateWidgetPosition(button, x, y);

	return button;
}

void MenuWidget::setButtonEnabled(const std::string &text, bool isEnabled) {
	for (auto &it : m_buttons) {
		if (it.first.text() == text)
			it.first.setEnabled(isEnabled);
	}
}

SliderWidget &MenuWidget::addSlider(const std::string &text, const SliderWidget::CppCallback &callback, int min, int max, int initialValue) {
	int x = int((m_buttons.size() + m_sliders.size()) % m_width);
	int y = int((m_buttons.size() + m_sliders.size()) / m_width);

	m_sliders.emplace_back(std::piecewise_construct,
		std::forward_as_tuple(this), std::forward_as_tuple(x, y));

	SliderWidget &slider = m_sliders.back().first;
	slider.setText(text);
	slider.setCallback(callback);
	slider.setMinMaxValues(min, max);
	slider.setCurrentValue(initialValue);

	updateWidgetPosition(slider, x, y);

	return slider;
}

void MenuWidget::updateWidgetPosition(Widget &widget, int x, int y) {
	widget.setPosition(float(x * (widget.width() + m_horizontalSpacing)),
	                   float(y * (widget.height() + m_verticalSpacing)));

	if ((unsigned int)widget.getPosition().x + widget.width() > Widget::m_width) {
		Widget::m_width = (unsigned int)widget.getPosition().x + widget.width();
	}
	if ((unsigned int)widget.getPosition().y + widget.height() > Widget::m_height) {
		Widget::m_height = (unsigned int)widget.getPosition().y + widget.height();
	}
}

void MenuWidget::draw(gk::RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	for (auto &it : m_buttons) {
		if (!it.first.text().empty())
			target.draw(it.first, states);
	}

	for (auto &it : m_sliders) {
		target.draw(it.first, states);
	}
}

