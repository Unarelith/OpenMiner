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

#include <SFML/Graphics/RenderWindow.hpp>

#include "IntTypes.hpp"
#include "RenderTarget.hpp"

class Window : public sf::Window, public RenderTarget {
	public:
		void open(const std::string &caption, u16 width, u16 height);

		void clear();

	private:
		void initOpenGL();
};

#endif // WINDOW_HPP_
