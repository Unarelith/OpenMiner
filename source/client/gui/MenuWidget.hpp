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
#ifndef MENUWIDGET_HPP_
#define MENUWIDGET_HPP_

#include "TextButton.hpp"

struct GuiScaleChangedEvent;

class MenuWidget : public Widget {
	public:
		MenuWidget(Widget *parent = nullptr) : Widget(parent) {}
		MenuWidget(u16 width, u16 height, Widget *parent = nullptr);

		void reset(u16 width, u16 height);

		void onEvent(const sf::Event &event) override;

		void onGuiScaleChanged(const GuiScaleChangedEvent &event);

		TextButton &addButton(const std::string &text, const TextButton::CppCallback &callback);

	private:
		void updateButtonPosition(TextButton &button, int x, int y);

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		static constexpr u16 s_verticalSpacing = 5;
		static constexpr u16 s_horizontalSpacing = 5;

		u16 m_width = 1;
		u16 m_height = 1;

		std::vector<TextButton> m_buttons;
};

#endif // MENUWIDGET_HPP_
