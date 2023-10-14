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
#include "Chat.hpp"
#include "Client.hpp"
#include "Config.hpp"

Chat::Chat(Client &client) : m_client(client) {
	client.setCommandCallback(Network::Command::ChatMessage, [&](sf::Packet &packet) {
		u16 clientID;
		std::string message;
		packet >> clientID >> message;

		addChatMessage(clientID, message);
	});
}

void Chat::addChatMessage(u16 clientID, const std::string &message) {
	if (clientID != 0)
		m_chatMessages.emplace_back(message, m_posY, &m_client.getPlayer(clientID));
	else
		m_chatMessages.emplace_back(message, m_posY);

	m_posY += m_chatMessages.back().text().getSize().y + 1;

	move(0.f, (float)-m_chatMessages.back().text().getSize().y - 1.f);
}

void Chat::setMessageVisibility(bool areMessagesVisible) {
	for (auto &it : m_chatMessages)
		it.setVisible(areMessagesVisible);
}

void Chat::draw(gk::RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	for (auto &it : m_chatMessages)
		target.draw(it, states);
}

