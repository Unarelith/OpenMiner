/*
 * =====================================================================================
 *
 *       Filename:  Application.hpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 05:09:11
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "ApplicationStateStack.hpp"
#include "GameClock.hpp"
#include "Renderer.hpp"

class Application {
	public:
		Application();

		void handleEvents();

		void run();

		Window &window() { return m_window; }

	private:
		ApplicationStateStack &m_stateStack;

		GameClock m_clock;

		Renderer m_renderer;

		Window m_window;
};

#endif // APPLICATION_HPP_
