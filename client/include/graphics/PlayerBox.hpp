/*
 * =====================================================================================
 *
 *       Filename:  PlayerBox.hpp
 *
 *    Description:
 *
 *        Created:  17/03/2019 15:32:46
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef PLAYERBOX_HPP_
#define PLAYERBOX_HPP_

#include <gk/gl/Drawable.hpp>
#include <gk/gl/VertexBuffer.hpp>
#include <gk/gl/Transformable.hpp>

#include "Player.hpp"

class PlayerBox : public gk::Drawable, public gk::Transformable, public Player {
	public:
		PlayerBox();

		void setPosition(float x, float y, float z) {
			gk::Transformable::setPosition(x, y, z);
			Player::setPosition(x, y, z);
		}

	private:
		void updateVertexBuffer();

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::VertexBuffer m_vbo;
};

#endif // PLAYERBOX_HPP_
