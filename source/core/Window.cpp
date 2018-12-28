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
	sf::ContextSettings settings;
	settings.majorVersion = 2;
	settings.minorVersion = 1;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;

	create(sf::VideoMode(width, height), caption, sf::Style::Titlebar | sf::Style::Close, settings);
	setPosition({int(sf::VideoMode::getDesktopMode().width / 2  - width / 2),
			int(sf::VideoMode::getDesktopMode().height / 2 - height / 2)});

	initOpenGL();
}

void Window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::initOpenGL() {
#ifdef __MINGW32__
#ifdef USE_GLAD
	if(!gladLoadGL()) {
		throw EXCEPTION("glad init failed");
	}
#else
	if(glewInit() != GLEW_OK) {
		throw EXCEPTION("glew init failed");
	}
#endif
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	// glBlendFunc(GL_ONE, GL_ONE);
	// glBlendEquation(GL_FUNC_ADD);
	// glEnable(GL_ALPHA_TEST);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glClearColor(0.196078, 0.6, 0.8, 1.0); // Skyblue
}

