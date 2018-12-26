/*
 * =====================================================================================
 *
 *       Filename:  Text.hpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 01:48:24
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <string>

#include "Sprite.hpp"

class Text : public IDrawable, public Transformable {
	public:
		Text();

		const std::string &text() const { return m_text; }
		void setText(const std::string &text) { m_text = text; updateTextSprites(); }

		void setColor(const Color &color) { m_color = color; }

		const Vector2i &getSize() const { return m_size; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		void updateTextSprites();
		void updateCharWidth();

		std::string m_text;
		std::vector<Sprite> m_textSprites;

		int m_charWidth[256];

		sf::Texture &m_texture;
		VertexBuffer m_vbo;

		Vector2i m_size;

		Color m_color = Color::white;
};

#endif // TEXT_HPP_
