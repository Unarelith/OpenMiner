/*
 * =====================================================================================
 *
 *       Filename:  Window.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/12/2014 00:16:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "SDLHeaders.hpp"
#include "Types.hpp"

class Window {
	public:
		Window();
		~Window();
		
		void open(std::string caption, u16 width, u16 height);
		
		void clear();
		void display();
		
		void setMouseCursorGrabbed(bool grabbed);
		void setMouseCursorVisible(bool visible);
		
		void setVerticalSyncEnabled(bool enabled);
		
		u16 width() const { return m_width; }
		u16 height() const { return m_height; }
		
		void close() { m_isOpen = false; }
		bool isOpen() const { return m_isOpen; }
		
	private:
		SDL_Window *m_window;
		SDL_GLContext m_glContext;
		
		u16 m_width;
		u16 m_height;
		
		bool m_isOpen;
};

#endif // WINDOW_HPP_
