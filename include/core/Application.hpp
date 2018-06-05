/*
 * =====================================================================================
 *
 *       Filename:  Application.hpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  14/12/2014 05:09:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
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

		static Application &getInstance() {
			static Application instance;
			return instance;
		}

	private:
		ApplicationStateStack *m_applicationStateStack;

		GameClock m_clock;

		Renderer m_renderer;

		Window m_window;
};

#endif // APPLICATION_HPP_
