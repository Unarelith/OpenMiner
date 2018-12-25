/*
 * =====================================================================================
 *
 *       Filename:  Transformable.hpp
 *
 *    Description:
 *
 *        Created:  11/11/2018 18:06:05
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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
		void setPosition(const Vector3f &position) { setPosition(position.x, position.y, position.z); }

		void setOrigin(float x, float y, float z = 0);
		void setOrigin(const Vector3f &origin) { setOrigin(origin.x, origin.y, origin.z); }

		void setScale(float factorX, float factorY, float factorZ = 1);
		void setScale(const Vector3f &factors) { setScale(factors.x, factors.y, factors.z); }

		void setRotation(float angle) { setRotation(angle, {0, 0, 1}); }
		void setRotation(float angle, const Vector3f &axis);

		const Vector3f& getPosition() const { return m_position; }
		const Vector3f& getOrigin() const { return m_origin; }
		const Vector3f& getScale() const { return m_scale; }
		float getRotation() const { return m_rotation; }

		void move(float offsetX, float offsetY, float offsetZ = 0);
		void move(const Vector3f &offset);

		void scale(float factorX, float factorY, float factorZ = 1);
		void scale(const Vector3f &factor);

		void rotate(float angle);
		void rotate(float angle, const Vector3f &axis);

		const Transform& getTransform() const;
		const Transform& getInverseTransform() const;

	private:
		Vector3f m_position{0, 0, 0};
		Vector3f m_origin{0, 0, 0};
		Vector3f m_scale{1, 1, 1};
		float m_rotation = 0;

		mutable Transform m_transform;
		mutable Transform m_inverseTransform;
		mutable Transform m_rotationTransform;

		mutable bool m_transformNeedUpdate = true;
		mutable bool m_inverseTransformNeedUpdate = true;
};

#endif // TRANSFORMABLE_HPP_
