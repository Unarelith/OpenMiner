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
#include "Crosshair.hpp"
#include "Skybox.hpp"
#include "World.hpp"

#include "RenderTarget.hpp"

class GameState : public ApplicationState {
	public:
		GameState();

		void updateCursorBlockData();

		void onEvent(const SDL_Event &event) override;

		void update() override;

		float fract(float value) const;
		glm::vec4 findSelectedBlock(bool useDepthBuffer) const;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;

		Camera &m_camera;

		Shader m_shader;

		Skybox m_skybox;
		World m_world;

		glm::vec4 m_selectedBlock{99999, 99999, 99999, -1};

		VertexBuffer m_cursorVBO{GL_LINES, 0, 24};
		Crosshair m_crosshair;
};

#endif // GAMESTATE_HPP_
