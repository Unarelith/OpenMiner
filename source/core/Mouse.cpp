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

void Mouse::resetToWindowCenter() {
#ifdef USE_SDL
	SDL_WarpMouseInWindow(s_window->window(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
#elif defined USE_SFML
    sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), *s_window->window());
//    sf::Mouse::setPosition({s_window->window()->getSize() / 2}, *s_window->window());
#endif // USE_SDL, USE_SFML
}

#ifdef USE_SFML
sf::Vector2i Mouse::getPosition() {
    return sf::Mouse::getPosition(*s_window->window());
}
#endif // USE_SFML

void Mouse::setCursorGrabbed(bool grabbed) {
#ifdef USE_SDL
	SDL_SetRelativeMouseMode((SDL_bool)grabbed);
#elif defined USE_SFML
    s_window->window()->setMouseCursorGrabbed(grabbed);
#endif // USE_SDL, USE_SFML
}

void Mouse::setCursorVisible(bool visible) {

#ifdef USE_SDL
	SDL_ShowCursor(visible);
#elif defined USE_SFML
    s_window->window()->setMouseCursorVisible(visible);
#endif // USE_SDL, USE_SFML
}

