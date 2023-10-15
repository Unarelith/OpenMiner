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
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

Camera::Camera(float fov, float near, float far) {
	m_fieldOfView = fov;
	m_nearPlane = near;
	m_farPlane = far;

	setDPosition(0, 0, 0);
}

void Camera::setPosition(const gk::Vector3f &position) {
	m_position = position;
	m_viewTransformUpdated = false;
}

void Camera::setDPosition(const gk::Vector3d &position) {
	m_position_d = position;
	m_position = gk::Vector3f((float)position.x, (float)position.y, (float)position.z);
	m_viewTransformUpdated = false;
}

void Camera::setDirection(const gk::Vector3f &direction) {
	m_direction = direction;
	m_viewTransformUpdated = false;
}

void Camera::setTargetPosition(const gk::Vector3f &target) {
	m_direction = target - getPosition();
	m_viewTransformUpdated = false;
}

void Camera::setUpVector(const gk::Vector3f &upVector) {
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

const gk::Transform& Camera::getTransform() const {
	if (!m_transformUpdated) {
		m_transform = glm::perspective(glm::radians(m_fieldOfView), m_aspect, m_nearPlane, m_farPlane);

		m_transformUpdated = true;
	}

	return m_transform;
}

const gk::Transform& Camera::getViewTransform() const {
	if (!m_viewTransformUpdated) {
		m_viewTransform = glm::lookAt(glm::vec3{getPosition().x, getPosition().y, getPosition().z},
		                              glm::vec3{getPosition().x + m_direction.x, getPosition().y + m_direction.y, getPosition().z + m_direction.z},
		                              glm::vec3{m_upVector.x, m_upVector.y, m_upVector.z});

		m_viewTransformUpdated = true;
	}

	return m_viewTransform;
}
