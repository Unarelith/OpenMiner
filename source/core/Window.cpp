/*
 * =====================================================================================
 *
 *       Filename:  Window.cpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 00:17:10
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "OpenGL.hpp"
#include "Exception.hpp"
#include "Window.hpp"

void Window::open(const std::string &caption, u16 width, u16 height) {
	sf::ContextSettings settings(/*depthBits:*/24, /*stencilBits:*/8, /*antialiasingLevel:*/4);
	settings.majorVersion = 2;  //OpenGL major Version
	settings.minorVersion = 1;  //OpenGL minor Version
	//For OpenGL version >=3.2 with Core profile set settings.attributeFlags = sf::ContextSettings::Core;

	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), caption, sf::Style::Titlebar | sf::Style::Close, settings);
	//SFML prints error message automatically if failed. Redirect sf::err() ?
	m_window->setPosition({int(sf::VideoMode::getDesktopMode().width/2  - width/2),
						   10}); //int(sf::VideoMode::getDesktopMode().height/2 - height/2)});  //FIXME: Cannot center properly. How to subtract height of title bar?

	m_width = width;
	m_height = height;

	m_isOpen = true;
}

void Window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display() {
	m_window->display();
}

void Window::setVerticalSyncEnabled(bool enabled) {
	m_window->setVerticalSyncEnabled(enabled);
}

