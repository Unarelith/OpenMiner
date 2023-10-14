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
#ifndef TEXTBUTTON_HPP_
#define TEXTBUTTON_HPP_

#include <functional>

#include <sol/sol.hpp>

#include "Image.hpp"
#include "Text.hpp"
#include "Widget.hpp"

class TextButton : public Widget {
	public:
		using CppCallback = std::function<void(TextButton &)>;
		using LuaCallback = sol::function;

		TextButton(Widget *parent = nullptr);
		TextButton(u16 width, Widget *parent = nullptr);
		TextButton(const CppCallback &callback, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		const std::string &text() const { return m_text.string(); }
		void setText(const std::string &text);

		void setCallback(const CppCallback &callback) { m_cppCallback = callback; }
		void setCallback(const LuaCallback &callback) { m_luaCallback = callback; }

		void setEnabled(bool isEnabled) {
			m_isEnabled = isEnabled;
			m_text.setColor(isEnabled ? m_defaultColor : m_disabledColor);
			m_text.setShadowEnabled(isEnabled);
		}

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		const gk::Color m_defaultColor = gk::Color::fromRGBA32(224, 224, 224);
		const gk::Color m_hoverColor = gk::Color::fromRGBA32(255, 255, 160);
		const gk::Color m_disabledColor = gk::Color::fromRGBA32(160, 160, 160);

		Image m_background{"texture-widgets"};
		Image m_hoverBackground{"texture-widgets"};
		Image m_disabledBackground{"texture-widgets"};

		Image m_backgroundBorder{"texture-widgets"};
		Image m_hoverBackgroundBorder{"texture-widgets"};
		Image m_disabledBackgroundBorder{"texture-widgets"};

		Text m_text;

		CppCallback m_cppCallback;
		LuaCallback m_luaCallback;

		bool m_isHovered = false;
		bool m_isEnabled = true;
};

#endif // TEXTBUTTON_HPP_
