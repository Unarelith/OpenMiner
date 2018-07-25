/*
 * =====================================================================================
 *
 *       Filename:  Mouse.cpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 15:39:17
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "Mouse.hpp"

Window *Mouse::s_window = nullptr;

Vector2i Mouse::resetToWindowCenter() {
	const Vector2i newPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	sf::Mouse::setPosition(sf::Vector2i(newPos.x, newPos.y), *s_window->window());
//	sf::Mouse::setPosition({s_window->window()->getSize() / 2}, *s_window->window());
	return newPos;
}

Vector2i Mouse::getPosition() {
	const sf::Vector2i pos = sf::Mouse::getPosition(*s_window->window());
	return {pos.x, pos.y};
}

void Mouse::setCursorGrabbed(bool grabbed) {
	s_window->window()->setMouseCursorGrabbed(grabbed);
}

void Mouse::setCursorVisible(bool visible) {
	s_window->window()->setMouseCursorVisible(visible);
}

