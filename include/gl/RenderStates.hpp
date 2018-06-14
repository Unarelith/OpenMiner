/*
 * =====================================================================================
 *
 *       Filename:  RenderStates.hpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 19:52:27
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef RENDERSTATES_HPP_
#define RENDERSTATES_HPP_

#include <glm/matrix.hpp>

class Texture;
class Shader;

struct RenderStates {
	glm::mat4 *projectionMatrix = nullptr;
	glm::mat4 *viewMatrix = nullptr;
	glm::mat4 *modelMatrix = nullptr;

	const Texture *texture = nullptr;
	const Shader *shader = nullptr;

	static const RenderStates Default;
};

#endif // RENDERSTATES_HPP_
