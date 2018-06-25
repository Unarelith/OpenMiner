/*
 * =====================================================================================
 *
 *       Filename:  RectangleShape.hpp
 *
 *    Description:
 *
 *        Created:  27/09/2014 17:06:48
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#ifndef RECTANGLESHAPE_HPP_
#define RECTANGLESHAPE_HPP_

#include "Color.hpp"
#include "IDrawable.hpp"
#include "Rect.hpp"
#include "Transformable.hpp"
#include "VertexBuffer.hpp"

class RectangleShape : public IDrawable, public Transformable {
	public:
		RectangleShape() = default;
		RectangleShape(u16 width, u16 height);

		const Color &color() const { return m_color; }
		void setColor(const Color &color) { m_color = color; updateVertexBuffer(); }

		void setWireframeMode(bool wireframeMode) { m_wireframeMode = wireframeMode; }

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }

		void setSize(u16 width, u16 height) { m_width = width; m_height = height; updateVertexBuffer(); }

	private:
		void updateVertexBuffer() const;

		void draw(RenderTarget &target, RenderStates states) const override;

		Color m_color = Color::black;

		bool m_wireframeMode = false;

		float m_width = 0;
		float m_height = 0;

		VertexBuffer m_vbo;
};

#endif // RECTANGLESHAPE_HPP_
