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
#ifndef SERVERLOADINGSTATE_HPP_
#define SERVERLOADINGSTATE_HPP_

#include <gk/core/ApplicationState.hpp>
#include <gk/gl/Shader.hpp>
#include <gk/graphics/Text.hpp>

#include "GameState.hpp"

class ServerLoadingState : public gk::ApplicationState {
	public:
		ServerLoadingState(GameState &game);

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Shader m_shader;

		gk::Text m_text;
		gk::Text m_textShadow;

		GameState &m_game;

		bool m_isWorldSent = false;
};

#endif // SERVERLOADINGSTATE_HPP_
