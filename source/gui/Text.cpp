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

Text::Text() : m_texture(ResourceHandler::getInstance().get<Texture>("texture-font")) {
	updateCharWidth();
}

void Text::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	for(const Sprite &sprite : m_textSprites) {
		target.draw(sprite, states);
	}
}

// FIXME: USE A VBO INSTEAD
void Text::updateTextSprites() {
	m_textSprites.clear();

	int x = 0;
	Color color = Color{70, 70, 70, 255};
	for(char c : m_text) {
		Sprite sprite{"texture-font", 8, 8};
		sprite.setCurrentFrame(c);
		sprite.setPosition(x + 1, 1, 0);
		sprite.setColor(color);
		m_textSprites.emplace_back(std::move(sprite));
		x += m_charWidth[(u8)c];
	}
	x = 0;
	color = m_color;
	for(char c : m_text) {
		Sprite sprite{"texture-font", 8, 8};
		sprite.setCurrentFrame(c);
		sprite.setPosition(x, 0, 0);
		if (c == '[')
			color = Color::blue;
		sprite.setColor(color);
		m_textSprites.emplace_back(std::move(sprite));
		x += m_charWidth[(u8)c];
	}

	m_size.x = x;
	m_size.y = 8;
}

// FIXME: Since I use the font from Minecraft assets, I needed to use
//        this piece of code to make it look good
//        I'll remove it later anyway
void Text::updateCharWidth() {
	const int width = m_texture.width();
	const int height = m_texture.height();
	unsigned int data[width * height];

	Texture::bind(&m_texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &data);
	Texture::bind(nullptr);

	const int charMaxHeight = height / 16;
	const int charMaxWidth = width / 16;

	for (int i = 0 ; i < 256 ; ++i) {
		if (i == ' ') {
			m_charWidth[i] = 4;
			continue;
		}

		int charX = i % 16;
		int charY = i / 16;

		if (i == 32)
			m_charWidth[i] = 4;

		int l1;
		for (l1 = charMaxWidth - 1 ; l1 >= 0 ; --l1) {
			int i2 = charX * charMaxWidth + l1;
			bool flag1 = true;

			for (int j2 = 0 ; j2 < charMaxHeight && flag1 ; ++j2) {
				int k2 = (charY * charMaxWidth + j2) * width;

				if ((data[i2 + k2] & 255) != 0)
					flag1 = false;
			}

			if (!flag1) break;
		}

		++l1;
		m_charWidth[i] = 0.5f + l1 * (8.0f / charMaxWidth) + 1;
	}
}

