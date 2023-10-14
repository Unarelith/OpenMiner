/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <gk/core/Config.hpp>
#include <gk/core/Utils.hpp>
#include <gk/gl/GLCheck.hpp>
#include <gk/gl/OpenGL.hpp>
#include <gk/core/Exception.hpp>

#include "Window.hpp"

void Window::open(const std::string &caption, u16 width, u16 height) {
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

	m_size.x = width;
	m_size.y = height;

	m_defaultView.reset(gk::FloatRect{0, 0, (float)width, (float)height});
	setView(m_defaultView);

	m_isOpen = true;

#ifdef GK_SYSTEM_WINDOWS
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

	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	glCheck(glEnable(GL_TEXTURE_2D));
}

void Window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display() {
	SDL_GL_SwapWindow(m_window.get());
}

void Window::onEvent(const SDL_Event &event) {
	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		glViewport(0, 0, event.window.data1, event.window.data2);

		m_size.x = (unsigned int)event.window.data1;
		m_size.y = (unsigned int)event.window.data2;
	}
}

void Window::setVerticalSyncEnabled(bool isVerticalSyncEnabled) {
	if (isVerticalSyncEnabled && !m_isVerticalSyncEnabled) {
		// Note: Adaptative sync (-1) causes stuttering in some cases
		if (/* SDL_GL_SetSwapInterval(-1) == -1 && */ SDL_GL_SetSwapInterval(1) == -1)
			gkWarning() << "Can't enable VSync";

	}
	else if (!isVerticalSyncEnabled && m_isVerticalSyncEnabled) {
		if (SDL_GL_SetSwapInterval(0) == -1)
			gkWarning() << "Can't disable VSync";
	}

	m_isVerticalSyncEnabled = isVerticalSyncEnabled;
}

void Window::setWindowMode(Mode mode) {
	if (m_windowMode != mode) {
		if (mode == Mode::Windowed) {
			SDL_SetWindowFullscreen(m_window.get(), 0);
			SDL_SetWindowSize(m_window.get(), (int)m_baseSize.x, (int)m_baseSize.y);
			SDL_SetWindowPosition(m_window.get(), m_basePosition.x, m_basePosition.y);
		}
		else {
			m_baseSize = m_size;

			SDL_GetWindowPosition(m_window.get(), &m_basePosition.x, &m_basePosition.y);
			SDL_SetWindowFullscreen(m_window.get(), (mode == Mode::Fullscreen) ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_FULLSCREEN_DESKTOP);

			SDL_DisplayMode desktopMode;
			if (SDL_GetDesktopDisplayMode(0, &desktopMode) == 0) {
				SDL_SetWindowDisplayMode(m_window.get(), &desktopMode);
				SDL_SetWindowSize(m_window.get(), desktopMode.w, desktopMode.h);
			}
			else
				gkError() << "Failed to set fullscreen mode";
		}

		m_windowMode = mode;
	}
}

gk::Vector2u Window::getSize() const {
	if (m_windowMode == Mode::Windowed)
		return m_size;
	else {
		int w, h;
		SDL_GetWindowSize(m_window.get(), &w, &h);

		return gk::Vector2u{
			static_cast<unsigned int>(w),
			static_cast<unsigned int>(h)
		};
	}
}

void Window::resize(unsigned int width, unsigned int height) {
	SDL_SetWindowSize(m_window.get(), (int)width, (int)height);
}

// From: https://stackoverflow.com/a/23091336/1392477
bool Window::saveScreenshot(int x, int y, int w, int h, const std::string &filename) noexcept {
	unsigned char *pixels = new unsigned char[(unsigned int)(w * h) * 4];
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Surface *surf = SDL_CreateRGBSurfaceFrom(pixels, w, h, 8 * 4, w * 4, rmask, gmask, bmask, amask);
	SDL_Surface *flip = gk::flipSDLSurface(surf);
	if (IMG_SavePNG(flip, filename.c_str()) < 0) {
		gkError() << "Failed to save texture to:" << filename;
		gkError() << "Reason:" << IMG_GetError();
		return false;
	}

	SDL_FreeSurface(flip);
	SDL_FreeSurface(surf);
	delete[] pixels;
	return true;
}

