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

#include "Client.hpp"
#include "ClientChunk.hpp"
#include "ClientCommandHandler.hpp"
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "Config.hpp"
#include "HUD.hpp"
#include "PlayerBox.hpp"

class GameState : public gk::ApplicationState {
	public:
		GameState(const std::string &host, int port);

		void onEvent(const SDL_Event &event) override;

		void update() override;

		Client &client() { return m_client; }

	private:
		void initShaders();

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Shader m_shader;

		gk::Camera m_camera{70.0f, DIST_NEAR, DIST_FAR};
		ClientPlayer m_player{m_camera};

		Client m_client;

		ClientWorld m_world;

		std::unordered_map<u16, PlayerBox> m_playerBoxes;

		ClientCommandHandler m_clientCommandHandler{m_client, m_world, m_player, m_camera, m_playerBoxes};

		HUD m_hud{m_player, m_world, m_clientCommandHandler};
};

#endif // GAMESTATE_HPP_
