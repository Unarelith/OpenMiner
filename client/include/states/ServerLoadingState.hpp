/*
 * =====================================================================================
 *
 *       Filename:  ServerLoadingState.hpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 21:57:24
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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
