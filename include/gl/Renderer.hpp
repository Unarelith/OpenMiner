/*
 * =====================================================================================
 *
 *       Filename:  Renderer.hpp
 *
 *    Description:
 *
 *        Created:  05/06/2018 15:40:44
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "Window.hpp"

class Renderer {
	public:
		void init(Window &window);

	private:
		Window *m_window = nullptr;
};

#endif // RENDERER_HPP_
