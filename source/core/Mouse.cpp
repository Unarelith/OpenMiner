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
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Config.hpp"
#include "Mouse.hpp"

Window *Mouse::s_window = nullptr;

sf::Vector2i Mouse::s_lastMousePos;
sf::Vector2f Mouse::s_lastDelta;

void Mouse::update(const sf::Event &event) {
	sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);

	auto windowSize = s_window->getSize();
	uint16_t aspectRatio = windowSize.x / windowSize.y;
	sf::Vector2i windowCenter(windowSize.x / 2, windowSize.y / 2);

	const float mouseSensitivity = 0.01f;
	s_lastDelta.x = (mousePos.x - s_lastMousePos.x) * mouseSensitivity * aspectRatio;
	s_lastDelta.y = -(mousePos.y - s_lastMousePos.y) * mouseSensitivity * aspectRatio;

	if (mousePos != windowCenter) {
		resetToWindowCenter();
		s_lastMousePos = windowCenter;
	}
	else {
		s_lastMousePos = mousePos;
	}
}

void Mouse::resetToWindowCenter() {
	auto windowSize = s_window->getSize();
	sf::Vector2i windowCenter(windowSize.x / 2, windowSize.y / 2);
	sf::Mouse::setPosition(windowCenter, *s_window);
}

sf::Vector2i Mouse::getPosition() {
	return sf::Mouse::getPosition(*s_window);
}

void Mouse::setCursorGrabbed(bool grabbed) {
	s_window->setMouseCursorGrabbed(grabbed);
}

void Mouse::setCursorVisible(bool visible) {
	s_window->setMouseCursorVisible(visible);
}

