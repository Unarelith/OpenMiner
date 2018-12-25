/*
 * =====================================================================================
 *
 *       Filename:  Transformable.cpp
 *
 *    Description:
 *
 *        Created:  11/11/2018 18:06:09
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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
	m_inverseTransformNeedUpdate = true;
}

void Transformable::setOrigin(float x, float y, float z) {
	m_origin.x = x;
	m_origin.y = y;
	m_origin.z = z;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transformable::setScale(float factorX, float factorY, float factorZ) {
	m_scale.x = factorX;
	m_scale.y = factorY;
	m_scale.z = factorZ;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transformable::setRotation(float angle, const Vector3f &axis) {
	m_rotation = std::fmod(angle, 360);
	if (m_rotation < 0)
		m_rotation += 360.f;

	m_rotationTransform = Transform::Identity;
	m_rotationTransform.rotate(m_rotation, axis);

	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transformable::move(float offsetX, float offsetY, float offsetZ) {
	setPosition(m_position.x + offsetX, m_position.y + offsetY, m_position.z + offsetZ);
}

void Transformable::move(const Vector3f &offset) {
	setPosition(m_position.x + offset.x, m_position.y + offset.y, m_position.z + offset.z);
}

void Transformable::scale(float factorX, float factorY, float factorZ) {
	setScale(m_scale.x * factorX, m_scale.y * factorY, m_scale.z * factorZ);
}

void Transformable::scale(const Vector3f &factor) {
	setScale(m_scale.x * factor.x, m_scale.y * factor.y, m_scale.z * factor.z);
}

void Transformable::rotate(float angle) {
	setRotation(m_rotation + angle);
}

void Transformable::rotate(float angle, const Vector3f &axis) {
	Transform newRotation;
	newRotation.rotate(angle, axis);

	m_rotationTransform = newRotation * m_rotationTransform;

	m_transformNeedUpdate = true;
}

const Transform& Transformable::getTransform() const {
	if (m_transformNeedUpdate) {
		Transform originTransform;
		originTransform.translate(-m_origin);

		Transform scaleTransform;
		scaleTransform.scale(m_scale);

		Transform positionTransform;
		positionTransform.translate(m_position);

		m_transform = positionTransform * scaleTransform * m_rotationTransform * originTransform;
		m_transformNeedUpdate = false;
	}

	return m_transform;
}

const Transform& Transformable::getInverseTransform() const {
	if (m_inverseTransformNeedUpdate) {
		m_inverseTransform = getTransform().getInverse();
		m_inverseTransformNeedUpdate = false;
	}

	return m_inverseTransform;
}

