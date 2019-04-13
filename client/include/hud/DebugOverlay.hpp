/*
 * =====================================================================================
 *
 *       Filename:  DebugOverlay.hpp
 *
 *    Description:
 *
 *        Created:  06/07/2018 13:54:09
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef DEBUGOVERLAY_HPP_
#define DEBUGOVERLAY_HPP_

#include "Text.hpp"

class ClientPlayer;

class DebugOverlay : public gk::Transformable, public gk::Drawable {
	public:
		DebugOverlay(const ClientPlayer &player);

		void update();

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		const ClientPlayer &m_player;

		Text m_versionText;
		Text m_positionText;
};

#endif // DEBUGOVERLAY_HPP_
