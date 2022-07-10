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
#ifndef RENDERINGSYSTEM_HPP_
#define RENDERINGSYSTEM_HPP_

#include <gk/gl/Camera.hpp>
#include <gk/gl/Drawable.hpp>
#include <gk/gl/Shader.hpp>

#include "Config.hpp"
#include "Framebuffer.hpp"
#include "HUD.hpp"
#include "PlayerBox.hpp"

class ClientWorld;
class Skybox;

class RenderingSystem : public gk::Drawable {
	public:
		RenderingSystem(gk::Camera &camera, ClientWorld &world, Skybox &skybox, std::unordered_map<u16, PlayerBox> &playerBoxes, HUD &hud, ClientPlayer &player)
			: m_camera(camera), m_world(world), m_skybox(skybox), m_playerBoxes(playerBoxes), m_hud(hud), m_player(player) {}

		void initShaders();

		void onEvent(const SDL_Event &event);

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		void setSkyColor() const;

		gk::Shader m_shader;

		Framebuffer m_fbo{Config::screenWidth, Config::screenHeight};

		gk::Camera &m_camera;
		ClientWorld &m_world;
		Skybox &m_skybox;
		std::unordered_map<u16, PlayerBox> &m_playerBoxes;
		HUD &m_hud;
		ClientPlayer &m_player;
};

#endif // RENDERINGSYSTEM_HPP_
