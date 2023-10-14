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
#ifndef SCROLLABLELIST_HPP_
#define SCROLLABLELIST_HPP_

#include <gk/graphics/Image.hpp>

#include "Text.hpp"
#include "Widget.hpp"

class ScrollableListElement : public Widget {
	public:
		ScrollableListElement(u16 id, const std::string &line1, const std::string &line2, const std::string &line3, Widget *parent);

		static u16 widgetWidth;

		u16 id() const { return m_id; }

		const std::string &line1() const { return m_line1.string(); }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		u16 m_id = 0;

		gk::Image m_icon{"texture-world_icon"};

		Text m_line1;
		Text m_line2;
		Text m_line3;
};

class ScrollableList : public Widget {
	public:
		ScrollableList();

		void onEvent(const SDL_Event &event) override;

		void addElement(const std::string &line1, const std::string &line2, const std::string &line3, bool isSelected = false);

		const ScrollableListElement *selectedElement() const { return m_selectedElement; }

	private:
		void selectElement(ScrollableListElement &element);
		void draw(RenderTarget &target, RenderStates states) const override;

		std::vector<ScrollableListElement> m_elements;

		RectangleShape m_cursor;
		ScrollableListElement *m_selectedElement = nullptr;
};

#endif // SCROLLABLELIST_HPP_
