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

class Player;

class DebugOverlay : public Transformable, public IDrawable {
	public:
		DebugOverlay(const Player &player);

		void update();

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		const Player &m_player;

		Text m_versionText;
		Text m_positionText;
};

#endif // DEBUGOVERLAY_HPP_
