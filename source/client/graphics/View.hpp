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
#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <gk/core/Rect.hpp>
#include <gk/core/Vector3.hpp>
#include <gk/gl/Transform.hpp>

class View {
	public:
		View();
		View(const gk::Vector3f &center, const gk::Vector2f &size);
		explicit View(const gk::FloatRect &rectangle);
		virtual ~View() = default;

		void setCenter(float x, float y, float z = 0);
		void setCenter(const gk::Vector3f &position) { setCenter(position.x, position.y, position.y); }

		void setSize(float width, float height);
		void setSize(const gk::Vector2f &size) { setSize(size.x, size.y); }

		void setRotation(float angle);

		void setViewport(const gk::FloatRect &viewport) { m_viewport = viewport; }

		void reset(const gk::FloatRect &rectangle);

		const gk::Vector3f &getCenter() const { return m_position; }
		const gk::Vector2f &getSize() const { return m_size; }

		float getRotation() const { return m_rotation; }

		const gk::FloatRect &getViewport() const { return m_viewport; }

		void move(float offsetX, float offsetY, float offsetZ = 0);
		void move(const gk::Vector3f &offset) { setCenter(m_position + offset); }

		void rotate(float angle) { setRotation(m_rotation + angle); }

		void zoom(float factor) { setSize(m_size.x * factor, m_size.y * factor); }

		virtual const gk::Transform &getTransform() const;
		virtual const gk::Transform &getViewTransform() const;

		virtual const gk::Vector3f &getPosition() const { return m_position; }

	protected:
		mutable bool m_transformUpdated = false;
		mutable bool m_viewTransformUpdated = false;

		mutable gk::Transform m_transform;
		mutable gk::Transform m_viewTransform;

		gk::Vector3f m_position;

	private:
		gk::Vector2f m_size;

		float m_rotation = 0.0f;

		gk::FloatRect m_viewport{0, 0, 1, 1};
};

#endif // VIEW_HPP_
