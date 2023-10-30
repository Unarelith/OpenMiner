/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "ChatMessage.hpp"
#include "Player.hpp"

ChatMessage::ChatMessage(const std::string &message, u32 posY, const Player *player) {
	if (player)
		m_text.setString("<" + player->name() + "> " + message);
	else
		m_text.setString(message);

	m_text.setPosition(0.f, (float)posY);
	m_text.setBackgroundColor(Color::fromRGBA32(0, 0, 0, 127));
	m_text.setBackgroundSize(300, 10);
	m_text.setMaxLineLength(300);
	m_text.setPadding(1, 1);
	m_text.updateVertexBuffer();

	m_timer.reset();
	m_timer.start();
}

void ChatMessage::draw(RenderTarget &target, RenderStates states) const {
	if (m_timer.time() <= 10000 || m_isVisible)
		target.draw(m_text, states);
}

