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
#ifndef SLIDERWIDGET_HPP_
#define SLIDERWIDGET_HPP_

#include <functional>

#include <gk/graphics/Image.hpp>

#include "Text.hpp"
#include "Widget.hpp"

class SliderWidget : public Widget {
	public:
		using CppCallback = std::function<void(SliderWidget &, u32)>;

		SliderWidget(Widget *parent = nullptr);
		SliderWidget(u16 width, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		int getCurrentValue() const { return m_min + (m_max - m_min) * m_percentage; }
		void setMinMaxValues(int min, int max) { m_min = min; m_max = max; }
		void setCurrentValue(int currentValue);

		const std::string &text() const { return m_text.string(); }
		void setText(const std::string &text);

		void setCallback(const CppCallback &callback) { m_callback = callback; }

	private:
		void updatePercentage(s32 mouseX);
		void updateSliderPosition();

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		const gk::Color m_defaultTextColor{224, 224, 224};
		const gk::Color m_hoverColor{255, 255, 160};

		gk::Image m_slider{"texture-widgets"};
		gk::Image m_background{"texture-widgets"};

		gk::Image m_sliderBorder{"texture-widgets"};
		gk::Image m_backgroundBorder{"texture-widgets"};

		Text m_text;

		int m_min = 0;
		int m_max = 100;

		float m_percentage = 0.f;

		bool m_isDragging = false;

		CppCallback m_callback;
};

#endif // SLIDERWIDGET_HPP_
