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
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "Config.hpp"
#include "HUD.hpp"
#include "Skybox.hpp"

class GameState : public gk::ApplicationState {
	public:
		GameState(Client &client, const std::string &host = "localhost", int port = 4242);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void initShaders();

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Shader m_shader;

		Skybox m_skybox;

		gk::Camera m_camera{FOV, DIST_NEAR, DIST_FAR};
		ClientPlayer m_player{m_camera};

		Client &m_client;

		HUD m_hud{m_player, m_world, m_client};

		ClientWorld m_world{m_client};

		bool m_isRegistryInitialized = false;
};

#endif // GAMESTATE_HPP_
