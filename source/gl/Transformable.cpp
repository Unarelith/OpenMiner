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
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3{x, y, z});
}

void Transformable::setPosition(float x, float y, float z) {
	m_modelMatrix = glm::translate(glm::mat4{1}, glm::vec3{x, y, z});
}

void Transformable::setScale(float factorX, float factorY, float factorZ) {
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{factorX, factorY, factorZ});
}

void Transformable::applyTransform(RenderStates &states) const {
	if (states.modelMatrix) {
		static glm::mat4 modelMatrix = *states.modelMatrix * getTransform();
		states.modelMatrix = &modelMatrix;
	}
	else {
		states.modelMatrix = &getTransform();
	}
}

