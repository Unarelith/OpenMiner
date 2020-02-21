/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "ChatMessage.hpp"

ChatMessage::ChatMessage(u16 clientID, const std::string &message, u32 messageCount) {
	m_text.setText("<Client " + std::to_string(clientID) + "> " + message);
	m_text.setPosition(0, 10 * messageCount);
	m_text.setBackgroundColor(gk::Color{0, 0, 0, 127});
	m_text.setBackgroundSize(300, 10);
	m_text.setPadding(1, 1);

	m_timer.reset();
	m_timer.start();
}

void ChatMessage::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_timer.time() <= 5000 || m_isVisible)
		target.draw(m_text, states);
}

