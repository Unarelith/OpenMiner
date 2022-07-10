/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
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
#include "KeyboardHandler.hpp"
#include "PlayerBox.hpp"
#include "Registry.hpp"
#include "RenderingSystem.hpp"
#include "Skybox.hpp"

class TextureAtlas;

class GameState : public gk::ApplicationState {
	public:
		GameState();

		void init() override;
		void onStateInactive() override;

		void connect(const std::string &host, u16 port, const std::string &username);

		void onEvent(const SDL_Event &event) override;

		void update() override;

		ClientPlayer &player() { return m_player; }
		Client &client() { return m_client; }
		ClientWorld &world() { return m_world; }
		ClientCommandHandler &clientCommandHandler() { return m_clientCommandHandler; }
		TextureAtlas &textureAtlas() { return m_textureAtlas; }

		void setSingleplayer(bool isSingleplayer) {
			m_clientCommandHandler.setSingleplayer(isSingleplayer);
			m_client.setSingleplayer(isSingleplayer);
		}

	private:
		void onGuiScaleChanged(const GuiScaleChangedEvent &event);

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Camera m_camera{70.0f, DIST_NEAR, DIST_FAR};
		ClientPlayer m_player{m_camera};

		Client m_client;

		std::unordered_map<u16, PlayerBox> m_playerBoxes;

		ClientCommandHandler m_clientCommandHandler{m_client, m_world, m_player, m_playerBoxes};

		HUD m_hud{m_player, m_world, m_clientCommandHandler};

		TextureAtlas &m_textureAtlas;

		Registry m_registry;

		ClientWorld m_world;

		KeyboardHandler *m_keyboardHandler;
		bool m_areModKeysLoaded = false;

		Skybox m_skybox{m_camera, m_world};

		RenderingSystem m_renderingSystem{m_camera, m_world, m_skybox, m_playerBoxes, m_hud, m_player};
};

#endif // GAMESTATE_HPP_
