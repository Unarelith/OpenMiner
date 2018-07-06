/*
 * =====================================================================================
 *
 *       Filename:  HUD.cpp
 *
 *    Description:
 *
 *        Created:  06/07/2018 13:58:46
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "HUD.hpp"

HUD::HUD(Camera &camera, Player &player, World &world, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
 : m_hotbar{player.hotbarInventory()},
   m_blockCursor(camera, player, world, viewMatrix, projectionMatrix),
   m_debugOverlay(camera)
{
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

}

void HUD::onEvent(const SDL_Event &event) {
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3)
		m_isDebugOverlayVisible ^= 1;

	m_hotbar.onEvent(event);
	m_blockCursor.onEvent(event, m_hotbar);
}

void HUD::update() {
	// FIXME: Shouldn't be called every tick
	m_hotbar.update();

	m_blockCursor.update(m_hotbar, false);

	m_debugOverlay.update();
}

void HUD::draw(RenderTarget &target, RenderStates states) const {
	target.draw(m_blockCursor, states);

	states.shader = &m_shader;
	states.vertexAttributes = VertexAttribute::Only2d;

	if (m_isDebugOverlayVisible)
		target.draw(m_debugOverlay, states);

	target.draw(m_hotbar, states);
	target.draw(m_crosshair, states);
}

