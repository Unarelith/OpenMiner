/*
 * =====================================================================================
 *
 *       Filename:  Renderer.cpp
 *
 *    Description:
 *
 *        Created:  05/06/2018 15:41:00
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "OpenGL.hpp"
#include "Renderer.hpp"

void Renderer::init(Window &window) {
	m_window = &window;

#ifdef __MINGW32__
	if(glewInit() != GLEW_OK) {
		throw EXCEPTION("glew init failed");
	}
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glClearColor(0.196078, 0.6, 0.8, 1.0); // Skyblue
	//glClearColor(0.0, 0.0, 0.0, 1.0);
}

