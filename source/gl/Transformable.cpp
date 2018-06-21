/*
 * =====================================================================================
 *
 *       Filename:  Transformable.cpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:35:59
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Transformable.hpp"

void Transformable::move(float x, float y, float z) {
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}

void Transformable::setPosition(float x, float y, float z) {
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Transformable::setScale(float factorX, float factorY, float factorZ) {
	m_scale.x = factorX;
	m_scale.y = factorY;
	m_scale.z = factorZ;
}

void Transformable::applyTransform(RenderStates &states) const {
	glm::mat4 &modelMatrix = const_cast<Transformable *>(this)->m_modelMatrix;
	modelMatrix = glm::mat4{1};
	modelMatrix = glm::translate(modelMatrix, m_position);
	modelMatrix = glm::scale(modelMatrix, m_scale);

	if (states.modelMatrix) {
		const_cast<Transformable *>(this)->m_tmpMatrix = *states.modelMatrix * m_modelMatrix;

		states.modelMatrix = &m_tmpMatrix;
	}
	else {
		states.modelMatrix = &m_modelMatrix;
	}
}

