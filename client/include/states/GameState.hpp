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
#include "Config.hpp"
#include "HUD.hpp"
#include "LuaCore.hpp"
#include "Player.hpp"
#include "Skybox.hpp"
#include "World.hpp"

class GameState : public gk::ApplicationState {
	public:
		GameState();

		void testLuaAPI();

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void initShaders();

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Shader m_shader;

		Skybox m_skybox;
		World m_world;

		gk::Camera m_camera{FOV, DIST_NEAR, DIST_FAR};
		Player m_player{m_camera};

		HUD m_hud{m_player, m_world};

		LuaCore m_luaCore;

		Client m_client;
};

#endif // GAMESTATE_HPP_
