/*
 * =====================================================================================
 *
 *       Filename:  CoreApplication.hpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 01:59:51
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef COREAPPLICATION_HPP_
#define COREAPPLICATION_HPP_

#include "ApplicationStateStack.hpp"
#include "GameClock.hpp"
#include "KeyboardHandler.hpp"
#include "ResourceHandler.hpp"
#include "SDLLoader.hpp"
#include "Window.hpp"

class CoreApplication {
	public:
		CoreApplication(int argc, char **argv);

		virtual void init();

		int run();

	protected:
		void createWindow(u16 screenWidth, u16 screenHeight, const char *windowTitle);

		ApplicationStateStack m_stateStack;

		GameClock m_clock;

		KeyboardHandler m_keyboardHandler;

		ResourceHandler m_resourceHandler;

		SDLLoader m_sdlLoader;

		Window m_window;


	private:
		void handleEvents();

		void mainLoop();
};

#endif // COREAPPLICATION_HPP_
