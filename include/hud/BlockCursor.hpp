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

#include <gk/core/SDLHeaders.hpp>

#include "Inventory.hpp"
#include "Player.hpp"
#include "World.hpp"

class Hotbar;

class BlockCursor : public gk::IDrawable {
	public:
		BlockCursor(Player &player, World &world, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
			: m_player(player), m_world(world), m_viewMatrix(viewMatrix), m_projectionMatrix(projectionMatrix) {}

		void onEvent(const SDL_Event &event, const Hotbar &hotbar);

		void update(const Hotbar &hotbar, bool useDepthBuffer);

		const Block *currentBlock() const { return m_currentBlock; }

	private:
		void updateVertexBuffer(const Block &block);
		void updateAnimationVertexBuffer(const Block &block, int animationPos = -1);

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		float fract(float value) const;
		glm::vec4 findSelectedBlock(bool useDepthBuffer) const;

		Player &m_player;
		World &m_world;
		glm::mat4 &m_viewMatrix;
		glm::mat4 &m_projectionMatrix;

		gk::VertexBuffer m_vbo;
		gk::VertexBuffer m_animationVBO;

		unsigned int m_animationStart = 0;
		glm::vec4 m_selectedBlock{0, 0, 0, 0};
		const Block *m_currentBlock = nullptr;
};

#endif // BLOCKCURSOR_HPP_
