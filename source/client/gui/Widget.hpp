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
#ifndef WIDGET_HPP_
#define WIDGET_HPP_

#include <SDL.h>

#include "Drawable.hpp"
#include "Rect.hpp"
#include "Transformable.hpp"

class Widget : public Drawable, public Transformable {
	public:
		Widget(Widget *parent = nullptr) : m_parent(parent) {}
		Widget(unsigned int width, unsigned int height, Widget *parent = nullptr)
			: m_parent(parent), m_width(width), m_height(height) {}

		virtual void onEvent(const SDL_Event &) {}
		virtual void update() {}

		bool isPointInWidget(int x, int y);

		FloatRect getGlobalBounds() const;

		const Widget *parent() const { return m_parent; }
		void setParent(Widget *parent) { m_parent = parent; }

		unsigned int width() const { return m_width; }
		unsigned int height() const { return m_height; }

	protected:
		void draw(RenderTarget &, RenderStates) const override {}

		Widget *m_parent = nullptr;

		unsigned int m_width = 0;
		unsigned int m_height = 0;
};

#endif // WIDGET_HPP_
