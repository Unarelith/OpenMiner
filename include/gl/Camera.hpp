/*
 * =====================================================================================
 *
 *       Filename:  Camera.hpp
 *
 *    Description:
 *
 *        Created:  26/12/2018 16:33:40
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "Transformable.hpp"

class Camera {
	public:
		Camera(float fov, float near, float far);
		virtual ~Camera() = default;

		const Vector3f &getPosition() const { return m_position; }
		const Vector3f &getDirection() const { return m_direction; }
		const Vector3f &getUpVector() const { return m_upVector; }

		float getFieldOfView() const { return m_fieldOfView; }
		float getAspectRatio() const { return m_aspect; }
		float getNearClippingPlane() const { return m_nearPlane; }
		float getFarClippingPlane() const { return m_farPlane; }

		void setPosition(float x, float y, float z) { setPosition(Vector3f(x, y, z)); }
		void setPosition(const Vector3f &position);

		void setDirection(float x, float y, float z) { setDirection(Vector3f(x, y, z)); }
		void setDirection(const Vector3f &direction);

		void setTargetPosition(float x, float y, float z) { setTargetPosition(Vector3f(x, y, z)); }
		void setTargetPosition(const Vector3f &target);

		void setUpVector(float x, float y, float z) { setUpVector(Vector3f(x, y, z)); }
		void setUpVector(const Vector3f &upVector);

		void setFieldOfView(float fov);
		void setAspectRatio(float aspect);
		void setNearClippingPlane(float distance);
		void setFarClippingPlane(float distance);

		virtual const Transform &getProjectionMatrix() const;
		virtual const Transform &getViewMatrix() const;

	private:
		Vector3f m_position{0, 0, 0};
		Vector3f m_direction{0, 0, -1};
		Vector3f m_upVector{0, 1, 0};

		float m_fieldOfView = 90.0f;
		float m_aspect = 1.0f;
		float m_nearPlane = 0.1f;
		float m_farPlane = 1000.0f;

		mutable Transform m_transform;
		mutable Transform m_viewTransform;

		mutable bool m_transformUpdated = false;
		mutable bool m_viewTransformUpdated = false;
};

#endif // CAMERA_HPP_
