/*
 * =====================================================================================
 *
 *       Filename:  Transformable.hpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:34:48
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef TRANSFORMABLE_HPP_
#define TRANSFORMABLE_HPP_

#include <glm/matrix.hpp>

#include "RenderStates.hpp"

class Transformable {
	public:
		void move(float x, float y, float z);
		void setPosition(float x, float y, float z);

		void setScale(float factorX, float factorY, float factorZ);

		void applyTransform(RenderStates &states) const;

		const glm::vec3 &getPosition() const { return m_position; }
		const glm::vec3 &getScale() const { return m_scale; }

	private:
		glm::mat4 m_modelMatrix{1};
		glm::mat4 m_tmpMatrix;

		glm::vec3 m_position{0, 0, 0};
		glm::vec3 m_scale{1, 1, 1};
};

#endif // TRANSFORMABLE_HPP_
