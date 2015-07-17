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
#include "Window.hpp"

class Application {
	public:
		Application();
		
		void initGL();
		
		void handleEvents();
		
		void run();
		
		Window &window() { return m_window; }
		
		static const u16 screenWidth = 640;
		static const u16 screenHeight = 480;
		
	private:
		ApplicationStateStack &m_stateStack;
		
		GameClock m_clock;
		
		Window m_window;
};

#endif // APPLICATION_HPP_
