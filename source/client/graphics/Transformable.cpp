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
#include <cmath>

#include "Transformable.hpp"

void Transformable::setPosition(float x, float y, float z) {
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	m_transformNeedUpdate = true;
	// m_inverseTransformNeedUpdate = true;
}

void Transformable::setOrigin(float x, float y, float z) {
	m_origin.x = x;
	m_origin.y = y;
	m_origin.z = z;
	m_transformNeedUpdate = true;
	// m_inverseTransformNeedUpdate = true;
}

void Transformable::setScale(float factorX, float factorY, float factorZ) {
	m_scale.x = factorX;
	m_scale.y = factorY;
	m_scale.z = factorZ;
	m_transformNeedUpdate = true;
	// m_inverseTransformNeedUpdate = true;
}

void Transformable::setRotation(float angle, const gk::Vector3f &axis) {
	m_rotation = std::fmod<float>(angle, 360.f);
	if (m_rotation < 0)
		m_rotation += 360.f;

	m_rotationTransform = gk::Transform::Identity;
	m_rotationTransform.rotate(m_rotation, axis);

	m_transformNeedUpdate = true;
	// m_inverseTransformNeedUpdate = true;
}

void Transformable::move(float offsetX, float offsetY, float offsetZ) {
	setPosition(m_position.x + offsetX, m_position.y + offsetY, m_position.z + offsetZ);
}

void Transformable::move(const gk::Vector3f &offset) {
	setPosition(m_position.x + offset.x, m_position.y + offset.y, m_position.z + offset.z);
}

void Transformable::scale(float factorX, float factorY, float factorZ) {
	setScale(m_scale.x * factorX, m_scale.y * factorY, m_scale.z * factorZ);
}

void Transformable::scale(const gk::Vector3f &factor) {
	setScale(m_scale.x * factor.x, m_scale.y * factor.y, m_scale.z * factor.z);
}

void Transformable::rotate(float angle) {
	setRotation(m_rotation + angle);
}

void Transformable::rotate(float angle, const gk::Vector3f &axis) {
	m_rotationTransform.rotate(angle, axis);

	m_transformNeedUpdate = true;
}

void Transformable::rotateX(float angle) {
	m_rotationTransform.rotateX(angle);

	m_transformNeedUpdate = true;
}

void Transformable::rotateY(float angle) {
	m_rotationTransform.rotateY(angle);

	m_transformNeedUpdate = true;
}

void Transformable::rotateZ(float angle) {
	m_rotationTransform.rotateZ(angle);

	m_transformNeedUpdate = true;
}

void Transformable::lrotateX(float angle) {
	m_rotationTransform.lrotateX(angle);

	m_transformNeedUpdate = true;
}

void Transformable::lrotateY(float angle) {
	m_rotationTransform.lrotateY(angle);

	m_transformNeedUpdate = true;
}

void Transformable::lrotateZ(float angle) {
	m_rotationTransform.lrotateZ(angle);

	m_transformNeedUpdate = true;
}

const gk::Transform& Transformable::getTransform() const {
	if (m_transformNeedUpdate) {
		gk::Transform originTransform;
		originTransform.translate(-m_origin);

		gk::Transform scaleTransform;
		scaleTransform.scale(m_scale);

		gk::Transform positionTransform;
		positionTransform.translate(m_position);

		m_transform = positionTransform * scaleTransform * m_rotationTransform * originTransform;
		m_transformNeedUpdate = false;
	}

	return m_transform;
}
