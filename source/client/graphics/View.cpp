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
#include <bx/math.h>

#include "EngineConfig.hpp"
#include "View.hpp"

View::View() {
	reset(FloatRect(0, 0, 1000, 1000));
}

View::View(const gk::Vector3f &center, const gk::Vector2f &size) {
	reset(FloatRect(center.xy(), size));
}

View::View(const FloatRect &rectangle) {
	reset(rectangle);
}

void View::setCenter(float x, float y, float z) {
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	m_viewTransformUpdated = false;
}

void View::setSize(float width, float height) {
	m_size.x = width;
	m_size.y = height;

	m_transformUpdated = false;
	m_viewTransformUpdated = false;
}

void View::setRotation(float angle) {
	m_rotation = static_cast<float>(fmod(angle, 360));
	if (m_rotation < 0)
		m_rotation += 360.f;

	m_viewTransformUpdated = false;
}

void View::reset(const FloatRect &rectangle) {
	m_position.x = rectangle.x + rectangle.sizeX / 2.f;
	m_position.y = rectangle.y + rectangle.sizeY / 2.f;

	m_size.x = rectangle.sizeX;
	m_size.y = rectangle.sizeY;

	m_rotation = 0;

	m_transformUpdated = false;
}

void View::move(float offsetX, float offsetY, float offsetZ) {
	setCenter(m_position.x + offsetX, m_position.y + offsetY, m_position.z + offsetZ);
}

const Transform &View::getTransform() const {
	if (!m_transformUpdated) {
		bx::mtxOrtho((float *)m_transform.getRawMatrix(), 0.0f, m_size.x, m_size.y, 0.0f, DIST_2D_NEAR, DIST_2D_FAR, 0, false);

		m_transformUpdated = true;
	}

	return m_transform;
}

const Transform &View::getViewTransform() const {
	if (!m_viewTransformUpdated) {
		Transform positionTransform;
		positionTransform.translate(m_size.x / 2.f - m_position.x, m_size.y / 2.f - m_position.y);

		Transform rotationTransform;
		rotationTransform.rotate(m_rotation);

		m_viewTransform = positionTransform * rotationTransform;

		// m_viewTransform = Transform::Identity;
		// m_viewTransform.rotate(m_rotation);
		// m_viewTransform.translate(Vector3f{getCenter().x, getCenter().y, 0}
		// 		- Vector3f{getSize(), 0.f} / 2.0f);

		m_viewTransformUpdated = true;
	}

	return m_viewTransform;
}
