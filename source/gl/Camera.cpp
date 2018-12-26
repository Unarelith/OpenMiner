/*
 * =====================================================================================
 *
 *       Filename:  Camera.cpp
 *
 *    Description:
 *
 *        Created:  26/12/2018 16:33:51
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

Camera::Camera(float fov, float near, float far) {
	m_fieldOfView = fov;
	m_nearPlane = near;
	m_farPlane = far;

	setPosition(0, 0, 0);
}

void Camera::setPosition(const Vector3f &position) {
	m_position = position;
	m_viewTransformUpdated = false;
}

void Camera::setDirection(const Vector3f &direction) {
	m_direction = direction;
	m_viewTransformUpdated = false;
}

void Camera::setTargetPosition(const Vector3f &target) {
	m_direction = target - getPosition();
	m_viewTransformUpdated = false;
}

void Camera::setUpVector(const Vector3f &upVector) {
	m_upVector = upVector;
	m_viewTransformUpdated = false;
}

void Camera::setFieldOfView(float fov) {
	m_fieldOfView = fov;
	m_transformUpdated = false;
}

void Camera::setAspectRatio(float aspect) {
	m_aspect = aspect;
	m_transformUpdated = false;
}

void Camera::setNearClippingPlane(float distance) {
	m_nearPlane = distance;
	m_transformUpdated = false;
}

void Camera::setFarClippingPlane(float distance) {
	m_farPlane = distance;
	m_transformUpdated = false;
}

const Transform& Camera::getProjectionMatrix() const {
	if (!m_transformUpdated) {
		m_transform = glm::perspective(m_fieldOfView, m_aspect, m_nearPlane, m_farPlane);

		m_transformUpdated = true;
	}

	return m_transform;
}

const Transform& Camera::getViewMatrix() const {
	if (!m_viewTransformUpdated) {
		m_viewTransform = glm::lookAt(glm::vec3{getPosition().x, getPosition().y, getPosition().z},
		                              glm::vec3{getPosition().x + m_direction.x, getPosition().y + m_direction.y, getPosition().z + m_direction.z},
		                              glm::vec3{m_upVector.x, m_upVector.y, m_upVector.z});

		m_viewTransformUpdated = true;
	}

	return m_viewTransform;
}

