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

#include "IDrawable.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

class Image : public IDrawable {
	public:
		Image() = default;
		Image(const Texture &texture);

		void load(const Texture &texture);

		void setClipRect(float x, float y, u16 width, u16 height);
		void setPosRect(float x, float y, u16 width, u16 height);

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }

	private:
		void updateVertexBuffer() const;

		void draw(RenderTarget &target, RenderStates states) const override;

		const Texture *m_texture = nullptr;

		u16 m_width = 0;
		u16 m_height = 0;

		FloatRect m_clipRect;
		FloatRect m_posRect;

		VertexBuffer m_vbo;
};

#endif // IMAGE_HPP_
