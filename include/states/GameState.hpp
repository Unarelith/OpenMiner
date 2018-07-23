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
#include "HUD.hpp"
#include "Player.hpp"
#include "Skybox.hpp"
#include "World.hpp"

#include "RenderTarget.hpp"

class GameState : public ApplicationState {
	public:
		GameState();

		void onEvent(const S_Event &event) override;

		void update() override;

	private:
		void initShaders();

		void draw(RenderTarget &target, RenderStates states) const override;

		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;

		Camera &m_camera{Camera::getInstance()};

		Shader m_shader;

		Skybox m_skybox;
		World m_world;

		Player m_player;

		HUD m_hud{m_camera, m_player, m_world, m_viewMatrix, m_projectionMatrix};
};

#endif // GAMESTATE_HPP_
