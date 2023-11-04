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
#ifndef TRANSFORMABLE_HPP_
#define TRANSFORMABLE_HPP_

#include "Transform.hpp"

class Transformable {
	public:
		virtual ~Transformable() = default;

		void setPosition(float x, float y, float z = 0);
		void setPosition(const gk::Vector3f &position) { setPosition(position.x, position.y, position.z); }

		void setOrigin(float x, float y, float z = 0);
		void setOrigin(const gk::Vector3f &origin) { setOrigin(origin.x, origin.y, origin.z); }

		void setScale(float factorX, float factorY, float factorZ = 1);
		void setScale(const gk::Vector3f &factors) { setScale(factors.x, factors.y, factors.z); }

		void setRotation(float angle) { setRotation(angle, {0, 0, 1}); }
		void setRotation(float angle, const gk::Vector3f &axis);

		const gk::Vector3f &getPosition() const { return m_position; }
		const gk::Vector3f &getOrigin() const { return m_origin; }
		const gk::Vector3f &getScale() const { return m_scale; }
		float getRotation() const { return m_rotation; }

		void move(float offsetX, float offsetY, float offsetZ = 0);
		void move(const gk::Vector3f &offset);

		void scale(float factorX, float factorY, float factorZ = 1);
		void scale(const gk::Vector3f &factor);

		void rotate(float angle);
		void rotate(float angle, const gk::Vector3f &axis);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		void lrotateX(float angle);
		void lrotateY(float angle);
		void lrotateZ(float angle);

		const Transform &getTransform() const;
		// const Transform &getInverseTransform() const;

		const Transform &getRotationTransform() const { return m_rotationTransform; }
		Transform &getRotationTransform() { return m_rotationTransform; }

	private:
		gk::Vector3f m_position{0, 0, 0};
		gk::Vector3f m_origin{0, 0, 0};
		gk::Vector3f m_scale{1, 1, 1};
		float m_rotation = 0;

		mutable Transform m_transform;
		// mutable Transform m_inverseTransform;
		mutable Transform m_rotationTransform;

		mutable bool m_transformNeedUpdate = true;
		// mutable bool m_inverseTransformNeedUpdate = true;
};

#endif // TRANSFORMABLE_HPP_
