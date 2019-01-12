/*
 * =====================================================================================
 *
 *       Filename:  Skybox.hpp
 *
 *    Description:
 *
 *        Created:  25/12/2014 23:43:07
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SKYBOX_HPP_
#define SKYBOX_HPP_

#include <gk/gl/IDrawable.hpp>
#include <gk/gl/VertexBuffer.hpp>

class Player;

class Skybox : public gk::IDrawable {
	public:
		Skybox();

		void update(Player &player);

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Vector3f m_playerPosition;

		gk::VertexBuffer m_vbo;
};

#endif // SKYBOX_HPP_
