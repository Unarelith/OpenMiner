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
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "client/graphics/Transform.hpp"

const Transform Transform::Identity;

Transform& Transform::combine(const Transform& transform) {
	m_matrix *= transform.m_matrix;
	return *this;
}

Transform& Transform::translate(float x, float y, float z) {
	m_matrix = glm::translate(m_matrix, {x, y, z});
	return *this;
}

Transform& Transform::rotate(float angle, const Vector3f& axis) {
	m_matrix = glm::rotate(m_matrix, glm::radians(angle), {axis.x, axis.y, axis.z});
	return *this;
}

Transform& Transform::rotateX(float angle) {
	float c = cosf(glm::radians(angle));
	float s = sinf(glm::radians(angle));
	for (int i = 0; i < 4; ++i) {
		float tmp = m_matrix[1][i];
		m_matrix[1][i] =  tmp * c + m_matrix[2][i] * s;
		m_matrix[2][i] = -tmp * s + m_matrix[2][i] * c;
	}
	return *this;
}

Transform& Transform::rotateY(float angle) {
	float c = cosf(glm::radians(angle));
	float s = sinf(glm::radians(angle));
	for (int i = 0; i < 4; ++i) {
		float tmp = m_matrix[0][i];
		m_matrix[0][i] = tmp * c - m_matrix[2][i] * s;
		m_matrix[2][i] = tmp * s + m_matrix[2][i] * c;
	}
	return *this;
}

Transform& Transform::rotateZ(float angle) {
	float c = cosf(glm::radians(angle));
	float s = sinf(glm::radians(angle));
	for (int i = 0; i < 4; ++i) {
		float tmp = m_matrix[0][i];
		m_matrix[0][i] =  tmp * c + m_matrix[1][i] * s;
		m_matrix[1][i] = -tmp * s + m_matrix[1][i] * c;
	}
	return *this;
}

Transform& Transform::lrotateX(float angle) {
	float c = cosf(glm::radians(angle));
	float s = sinf(glm::radians(angle));
	for (int i = 0; i < 4; ++i) {
		float tmp = m_matrix[i][1];
		m_matrix[i][1] = tmp * c - m_matrix[i][2] * s;
		m_matrix[i][2] = tmp * s + m_matrix[i][2] * c;
	}
	return *this;
}

Transform& Transform::lrotateY(float angle) {
	float c = cosf(glm::radians(angle));
	float s = sinf(glm::radians(angle));
	for (int i = 0; i < 4; ++i) {
		float tmp = m_matrix[i][0];
		m_matrix[i][0] =  tmp * c + m_matrix[i][2] * s;
		m_matrix[i][2] = -tmp * s + m_matrix[i][2] * c;
	}
	return *this;
}

Transform& Transform::lrotateZ(float angle) {
	float c = cosf(glm::radians(angle));
	float s = sinf(glm::radians(angle));
	for (int i = 0; i < 4; ++i) {
		float tmp = m_matrix[i][0];
		m_matrix[i][0] = tmp * c - m_matrix[i][1] * s;
		m_matrix[i][1] = tmp * s + m_matrix[i][1] * c;
	}
	return *this;
}

Transform& Transform::scale(float scaleX, float scaleY, float scaleZ) {
	m_matrix = glm::scale(m_matrix, {scaleX, scaleY, scaleZ});
	return *this;
}

Transform operator*(const Transform& left, const Transform &right) {
	return Transform(left).combine(right);
}

Transform& operator*=(Transform &left, const Transform &right) {
	return left.combine(right);
}
