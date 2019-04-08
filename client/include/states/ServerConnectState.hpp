/*
 * =====================================================================================
 *
 *       Filename:  ServerConnectState.hpp
 *
 *    Description:
 *
 *        Created:  08/04/2019 18:50:38
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERCONNECTSTATE_HPP_
#define SERVERCONNECTSTATE_HPP_

#include <gk/graphics/TextInput.hpp>

#include "InterfaceState.hpp"
#include "TextButton.hpp"

class ServerConnectState : public InterfaceState {
	public:
		ServerConnectState();

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::TextInput m_textInput;

		TextButton m_connectButton;
};

#endif // SERVERCONNECTSTATE_HPP_
