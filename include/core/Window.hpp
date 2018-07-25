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

#include "IntTypes.hpp"
#include "RenderTarget.hpp"
#include "SFMLHeaders.hpp"

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

		sf::RenderWindow *window() const { return m_window.get(); }

	private:
		using SFML_WindowPtr = std::unique_ptr<sf::RenderWindow>;
//		using SFML_GLContextPtr = std::unique_ptr<sf::Context>;  //For multithreading?

		SFML_WindowPtr m_window{nullptr};
//		SFML_GLContextPtr m_context{nullptr};

		u16 m_width;
		u16 m_height;

		bool m_isOpen;
};

#endif // WINDOW_HPP_
