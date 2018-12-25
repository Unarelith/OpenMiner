/*
 * =====================================================================================
 *
 *       Filename:  Transform.cpp
 *
 *    Description:
 *
 *        Created:  11/11/2018 17:39:55
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Transform.hpp"

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
	float rotationAngle = angle * RADIANS_PER_DEGREES;
	m_matrix = glm::rotate(m_matrix, rotationAngle, {axis.x, axis.y, axis.z});
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

