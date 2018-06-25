/*
 * =====================================================================================
 *
 *       Filename:  Cube.hpp
 *
 *    Description:
 *
 *        Created:  26/06/2018 00:01:26
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CUBE_HPP_
#define CUBE_HPP_

#include "IDrawable.hpp"
#include "Transformable.hpp"
#include "VertexBuffer.hpp"

class Block;

class Cube : public IDrawable, public Transformable {
	public:
		Cube(float size = 1.0f);

		void updateVertexBuffer(const Block &block) const;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		float m_size = 1.0f;

		const Texture &m_texture;

		VertexBuffer m_vbo;
};

#endif // CUBE_HPP_
