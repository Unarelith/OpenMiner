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
class MessageBus;
class Skybox;

namespace RenderingEvent {
	struct DrawObjects {
		std::vector<const gk::Drawable *> objects;
		bool inFramebuffer;
	};
}

class RenderingSystem : public gk::Drawable {
	public:
		RenderingSystem(MessageBus &messageBus, gk::Camera &camera, ClientWorld &world, HUD &hud, Skybox &skybox);

		void initShaders();

		void update();

		void onEvent(const SDL_Event &event);

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		void onDrawObjects(const RenderingEvent::DrawObjects &event);

		void setSkyColor() const;

		MessageBus &m_messageBus;

		gk::Shader m_shader;

		Framebuffer m_fbo{Config::screenWidth, Config::screenHeight};

		gk::Camera &m_camera; // to draw the world
		ClientWorld &m_world; // for the sky
		HUD &m_hud; // hud resize on window size change
		Skybox &m_skybox; // load sky

		// event 0: with framebuffer | event 1: without framebuffer
		RenderingEvent::DrawObjects m_events[2];
};

#endif // RENDERINGSYSTEM_HPP_
