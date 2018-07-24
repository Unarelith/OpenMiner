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
#ifdef USE_SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	m_window.reset(SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
	if(!m_window) {
		throw EXCEPTION("Window initialization failed:", SDL_GetError());
	}

	m_context.reset(SDL_GL_CreateContext(m_window.get()));
	if(!m_context) {
		throw EXCEPTION("OpenGL context creation failed:", SDL_GetError());
	}
#elif defined USE_SFML
    sf::ContextSettings settings(/*depthBits:*/24, /*stencilBits:*/8, /*antialiasingLevel:*/4);
    settings.majorVersion = 2;  //OpenGL major Version
    settings.minorVersion = 1;  //OpenGL minor Version
    //For OpenGL version >=3.2 with Core profile set settings.attributeFlags = sf::ContextSettings::Core;

    m_window = std::make_unique<S_Window>(sf::VideoMode(width, height), caption, sf::Style::Titlebar | sf::Style::Close, settings);
    //SFML prints error message automatically if failed
    m_window->setPosition({int(sf::VideoMode::getDesktopMode().width/2  - width/2),
                           10});  //int(sf::VideoMode::getDesktopMode().height/2 - height/2)});  //How to subtract height of title bar?
#endif // USE_SDL, USE_SFML

	m_width = width;
	m_height = height;

	m_isOpen = true;
}

void Window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display() {
#ifdef USE_SDL
	SDL_GL_SwapWindow(m_window.get());
#elif defined USE_SFML
    m_window->display();
#endif // USE_SDL, USE_SFML
}

void Window::setVerticalSyncEnabled(bool enabled) {
#ifdef USE_SDL
	if(SDL_GL_SetSwapInterval(enabled) < 0) {
		DEBUG("Warning: Can't enable VSync");
	}
#elif defined USE_SFML
	m_window->setVerticalSyncEnabled(enabled);
#endif // USE_SDL, USE_SFML
}

