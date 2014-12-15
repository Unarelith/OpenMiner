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

#include <SFML/Graphics/RenderWindow.hpp>

#include "ApplicationStateStack.hpp"
#include "GameClock.hpp"
//#include "ResourceHandler.hpp"

class Application {
	public:
		Application();
		~Application();
		
		void handleEvents();
		
		void run();
		
		void resetView() { m_window.setView(m_defaultView); }
		
		sf::RenderWindow &window() { return m_window; }
		
		static Application &getInstance() {
			static Application instance;
			return instance;
		}
		
	private:
		sf::RenderWindow m_window;
		sf::View m_defaultView;
		
		GameClock m_clock;
		
		ApplicationStateStack *m_applicationStateStack;
};

#endif // APPLICATION_HPP_
