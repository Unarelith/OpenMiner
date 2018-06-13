/*
 * =====================================================================================
 *
 *       Filename:  GameState.hpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 03:51:32
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

#include <glm/glm.hpp>

#include "ApplicationState.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "World.hpp"

class GameState : public ApplicationState {
	public:
		GameState();

		void update();

		void draw();
		void drawSelectedBlock();
		void drawCross();

		float fract(float value);
		glm::vec4 findSelectedBlock(bool useDepthBuffer);

	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;

		Camera &m_camera;

		Shader m_shader;

		Skybox m_skybox;
		World m_world;

		glm::vec4 m_selectedBlock{99999, 99999, 99999, -1};

		GLuint m_cursorVBO;
};

#endif // GAMESTATE_HPP_
