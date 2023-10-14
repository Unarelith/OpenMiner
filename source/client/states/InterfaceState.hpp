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
#ifndef INTERFACESTATE_HPP_
#define INTERFACESTATE_HPP_

#include <gk/core/ApplicationState.hpp>
#include <gk/gl/Shader.hpp>

#include "RectangleShape.hpp"

class InterfaceState : public gk::ApplicationState {
	public:
		InterfaceState(gk::ApplicationState *parent = nullptr);

	protected:
		void setup();
		void onEvent(const SDL_Event &event) override;

		void update() override;

		void prepareDraw(RenderTarget &target, RenderStates &states) const;

		bool m_drawBackground = true;

	private:
		virtual void updateWidgetPosition() {}

		gk::Shader m_shader;
		// gk::View m_view;

		glm::mat4 m_projectionMatrix;

		RectangleShape m_background;
};

#endif // INTERFACESTATE_HPP_
