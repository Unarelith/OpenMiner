/*
 * =====================================================================================
 *
 *       Filename:  Application.cpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 05:09:21
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Application.hpp"
#include "Config.hpp"
#include "Mouse.hpp"

#include "TextureLoader.hpp"

#include "GameState.hpp"

void Application::init() {
	CoreApplication::init();

	createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
	m_window.setVerticalSyncEnabled(true);

	initOpenGL();

	Mouse::setCursorVisible(false);
	Mouse::setCursorGrabbed(true);

	m_resourceHandler.loadConfigFile<TextureLoader>("resources/config/textures.xml");

	Registry::setInstance(m_registry);
	m_registry.registerBlocks();
	m_registry.registerItems();
	m_registry.registerRecipes();

	m_stateStack.push<GameState>();
}

void Application::initOpenGL() {
#ifdef __MINGW32__
#ifdef USE_GLAD
	if(!gladLoadGL()) {
		throw EXCEPTION("OpenGL init failed");
	}
#else
	if(glewInit() != GLEW_OK) {
		throw EXCEPTION("glew init failed");
	}
#endif // USE_GLAD, else GLEW
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


