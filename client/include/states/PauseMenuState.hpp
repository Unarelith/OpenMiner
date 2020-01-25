/*
 * =====================================================================================
 *
 *       Filename:  PauseMenuState.hpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:14:02
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef PAUSEMENUSTATE_HPP_
#define PAUSEMENUSTATE_HPP_

#include <gk/graphics/RectangleShape.hpp>

#include "InterfaceState.hpp"
#include "MenuWidget.hpp"

class Client;

class PauseMenuState : public InterfaceState {
	public:
		PauseMenuState(Client &client, gk::ApplicationState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		MenuWidget m_menuWidget{1, 4};

		gk::RectangleShape m_background;

		Client &m_client;
};

#endif // PAUSEMENUSTATE_HPP_
