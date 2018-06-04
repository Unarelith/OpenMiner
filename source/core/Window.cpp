/*
 * =====================================================================================
 *
 *       Filename:  Window.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  20/12/2014 00:17:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
 *
 * =====================================================================================
 */
#include "OpenGL.hpp"
#include "Exception.hpp"
#include "Window.hpp"

Window::Window() {
}

Window::~Window() {
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
}

void Window::open(std::string caption, u16 width, u16 height) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	m_window = SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(!m_window) {
		throw EXCEPTION("Window initialization failed:", SDL_GetError());
	}

	m_glContext = SDL_GL_CreateContext(m_window);
	if(!m_glContext) {
		SDL_DestroyWindow(m_window);
		throw EXCEPTION("OpenGL context creation failed:", SDL_GetError());
	}

	m_width = width;
	m_height = height;

	m_isOpen = true;
}

void Window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display() {
	SDL_GL_SwapWindow(m_window);
}

void Window::setMouseCursorGrabbed(bool grabbed) {
	SDL_SetRelativeMouseMode((SDL_bool)grabbed);
}

void Window::setMouseCursorVisible(bool visible) {
	SDL_ShowCursor(visible);
}

void Window::setVerticalSyncEnabled(bool enabled) {
	if(SDL_GL_SetSwapInterval(enabled) < 0) {
		DEBUG("Warning: Can't enable VSync");
	}
}

