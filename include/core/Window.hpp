/*
 * =====================================================================================
 *
 *       Filename:  Window.hpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 00:16:51
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <memory>
#include <string>

#include "RenderTarget.hpp"
#include "SDLHeaders.hpp"
#include "Types.hpp"

class Window : public RenderTarget {
	public:
		void open(const std::string &caption, u16 width, u16 height);

		void clear();
		void display();

		void setVerticalSyncEnabled(bool enabled);

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }

		void close() { m_isOpen = false; }
		bool isOpen() const { return m_isOpen; }

		SDL_Window *window() const { return m_window.get(); }

	private:
		using SDL_WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
		using SDL_GLContextPtr = std::unique_ptr<void, decltype(&SDL_GL_DeleteContext)>;

		SDL_WindowPtr m_window{nullptr, SDL_DestroyWindow};
		SDL_GLContextPtr m_context{nullptr, SDL_GL_DeleteContext};

		u16 m_width;
		u16 m_height;

		bool m_isOpen;
};

#endif // WINDOW_HPP_
