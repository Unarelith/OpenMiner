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
#include "Chat.hpp"
#include "Client.hpp"
#include "Config.hpp"

Chat::Chat(Client &client) {
	setPosition(2, Config::screenHeight / Config::guiScale - 50);

	client.setCommandCallback(Network::Command::ChatMessage, [this](sf::Packet &packet) {
		u16 clientID;
		std::string message;
		packet >> clientID >> message;

		m_chatMessages.emplace_back(clientID, message, m_chatMessages.size());

		move(0, -10);
	});
}

void Chat::setMessageVisibility(bool areMessagesVisible) {
	for (auto &it : m_chatMessages)
		it.setVisible(areMessagesVisible);
}

void Chat::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	for (auto &it : m_chatMessages)
		target.draw(it, states);
}

