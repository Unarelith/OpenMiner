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

#include <gk/gui/Sprite.hpp>

class Text : public gk::IDrawable, public gk::Transformable {
	public:
		Text();

		const std::string &text() const { return m_text; }
		void setText(const std::string &text) { m_text = text; updateTextSprites(); }

		const gk::Color &color() const { return m_color; }
		void setColor(const gk::Color &color) { m_color = color; updateTextSprites(); }

		const gk::Vector2i &getSize() const { return m_size; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		void updateTextSprites();
		void updateCharWidth();

		std::string m_text;
		std::vector<gk::Sprite> m_textSprites;

		int m_charWidth[256];

		gk::Texture &m_texture;
		gk::VertexBuffer m_vbo;

		gk::Vector2i m_size;

		gk::Color m_color = gk::Color::White;
};

#endif // TEXT_HPP_
