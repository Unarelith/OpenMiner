/*
 * =====================================================================================
 *
 *       Filename:  HUD.hpp
 *
 *    Description:
 *
 *        Created:  06/07/2018 13:58:33
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef HUD_HPP_
#define HUD_HPP_

#include "BlockCursor.hpp"
#include "BlockInfoWidget.hpp"
#include "Crosshair.hpp"
#include "DebugOverlay.hpp"
#include "Hotbar.hpp"

class HUD : public gk::Transformable, public gk::IDrawable {
	public:
		HUD(Player &player, World &world, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);

		void onEvent(const SDL_Event &event);

		void update();

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Shader m_shader;

		glm::mat4 &m_projectionMatrix;

		Hotbar m_hotbar;

		glm::vec4 m_selectedBlock{0, 0, 0, -1};
		BlockCursor m_blockCursor;
		Crosshair m_crosshair;

		DebugOverlay m_debugOverlay;
		bool m_isDebugOverlayVisible = false;

		BlockInfoWidget m_blockInfoWidget;
};

#endif // HUD_HPP_
