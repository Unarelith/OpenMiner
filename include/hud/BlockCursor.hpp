/*
 * =====================================================================================
 *
 *       Filename:  BlockCursor.hpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 23:38:47
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCKCURSOR_HPP_
#define BLOCKCURSOR_HPP_

#include "Camera.hpp"
#include "Inventory.hpp"
#include "SDLHeaders.hpp"
#include "World.hpp"

class Hotbar;

class BlockCursor : public IDrawable {
	public:
		BlockCursor(Camera &camera, World &world, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
			: m_camera(camera), m_world(world), m_viewMatrix(viewMatrix), m_projectionMatrix(projectionMatrix)
		{ init(); }

		void init();

		void onEvent(const SDL_Event &event, Inventory &playerInventory, Inventory &hotbarInventory, const Hotbar &hotbar);

		void update(bool useDepthBuffer);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		float fract(float value) const;
		glm::vec4 findSelectedBlock(bool useDepthBuffer) const;

		Camera &m_camera;
		World &m_world;
		glm::mat4 &m_viewMatrix;
		glm::mat4 &m_projectionMatrix;

		VertexBuffer m_vbo;

		glm::vec4 m_selectedBlock;
};

#endif // BLOCKCURSOR_HPP_
