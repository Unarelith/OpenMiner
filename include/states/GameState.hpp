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

#include <gk/core/ApplicationState.hpp>
#include <gk/gl/RenderTarget.hpp>

#include "HUD.hpp"
#include "Player.hpp"
#include "Skybox.hpp"
#include "World.hpp"

class GameState : public gk::ApplicationState {
	public:
		GameState();

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void initShaders();

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		glm::mat4 m_perspectiveMatrix;
		glm::mat4 m_orthoMatrix;
		glm::mat4 m_viewMatrix;

		gk::Shader m_shader;

		Skybox m_skybox;
		World m_world;

		Player m_player;

		HUD m_hud{m_player, m_world, m_viewMatrix, m_orthoMatrix};
};

#endif // GAMESTATE_HPP_
