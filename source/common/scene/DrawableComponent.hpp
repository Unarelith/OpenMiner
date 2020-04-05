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
#ifndef DRAWABLECOMPONENT_HPP_
#define DRAWABLECOMPONENT_HPP_

#include <memory>

#include <gk/gl/Drawable.hpp>

class DrawableComponent {
	public:
		template<typename T, typename... Args>
		auto setDrawable(Args &&...args) -> typename std::enable_if<std::is_base_of<gk::Drawable, T>::value, T &>::type {
			m_drawable = std::make_shared<T>(std::forward<Args>(args)...);
			return *std::static_pointer_cast<T>(m_drawable);
		}

		void draw(gk::RenderTarget &target, gk::RenderStates states) {
			if (m_drawable)
				target.draw(*m_drawable, states);
		}

	private:
		std::shared_ptr<gk::Drawable> m_drawable;
};

#endif // DRAWABLECOMPONENT_HPP_
