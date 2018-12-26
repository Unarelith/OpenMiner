/*
 * =====================================================================================
 *
 *       Filename:  Image.hpp
 *
 *    Description:
 *
 *        Created:  20/09/2014 16:21:56
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <SFML/Graphics/Texture.hpp>

#include "Color.hpp"
#include "IDrawable.hpp"
#include "Transformable.hpp"
#include "VertexBuffer.hpp"

class Image : public IDrawable, public Transformable {
	public:
		Image() = default;
		Image(const std::string &textureName);
		Image(const sf::Texture &texture);

		void load(const std::string &textureName);
		void load(const sf::Texture &texture);

		const sf::FloatRect &clipRect() const { return m_clipRect; }
		void setClipRect(float x, float y, u16 width, u16 height);

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }

		void setColor(const Color &color) { m_color = color; updateVertexBuffer(); }

	private:
		void updateVertexBuffer() const;

		void draw(RenderTarget &target, RenderStates states) const override;

		const sf::Texture *m_texture = nullptr;

		u16 m_width = 0;
		u16 m_height = 0;

		sf::FloatRect m_clipRect;

		VertexBuffer m_vbo;

		Color m_color = Color::white;
};

#endif // IMAGE_HPP_
