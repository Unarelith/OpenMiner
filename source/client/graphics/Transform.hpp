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
#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm/gtc/type_ptr.hpp>

#include "Vector3.hpp"

class Transform {
	public:
		Transform() = default;
		Transform(const glm::mat4 &matrix) : m_matrix(matrix) {}

		Transform& combine(const Transform& transform);

		Transform& translate(float x, float y, float z = 0);
		Transform& translate(const Vector3f& offset) { return translate(offset.x, offset.y, offset.z); }

		// Intrinsic rotation (right multiplication)
		Transform& rotate(float angle) { return rotate(angle, {0, 0, 1}); }
		Transform& rotate(float angle, const Vector3f& axis);
		Transform& rotateX(float angle);
		Transform& rotateY(float angle);
		Transform& rotateZ(float angle);
		// Extrinsic rotation (left multiplication)
		Transform& lrotateX(float angle);
		Transform& lrotateY(float angle);
		Transform& lrotateZ(float angle);

		Transform& scale(float scaleX, float scaleY, float scaleZ = 1);
		Transform& scale(const Vector3f& factors) { return scale(factors.x, factors.y, factors.z); }

		const float* getRawMatrix() const { return glm::value_ptr(m_matrix); }
		const glm::mat4 &getMatrix() const { return m_matrix; }
		glm::mat4 &getMatrix() { return m_matrix; }

		// Transform getInverse() const { return glm::inverse(m_matrix); }
		// Transform getTranspose() const { return glm::transpose(m_matrix); }

		static const Transform Identity;

	private:
		glm::mat4 m_matrix{1};
};

Transform operator*(const Transform& left, const Transform& right);
Transform& operator*=(Transform& left, const Transform& right);

#endif // TRANSFORM_HPP_
