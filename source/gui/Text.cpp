/*
 * =====================================================================================
 *
 *       Filename:  Text.cpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 01:50:39
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ResourceHandler.hpp"
#include "Text.hpp"
#include "Texture.hpp"

#include "Sprite.hpp"

Text::Text() : m_texture(ResourceHandler::getInstance().get<Texture>("texture-font")) {
}

void Text::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	int i = 0;
	// FIXME: USE A VBO INSTEAD
	for(char c : m_text) {
		Sprite sprite{"texture-font", 8, 8};
		sprite.setCurrentFrame(c);
		sprite.setPosition(i * 6, 0, 0);
		target.draw(sprite, states);
		++i;
	}
}

