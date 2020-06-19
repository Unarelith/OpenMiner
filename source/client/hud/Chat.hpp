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
#ifndef CHAT_HPP_
#define CHAT_HPP_

#include <deque>

#include "ChatMessage.hpp"

class Client;

class Chat : public gk::Drawable, public gk::Transformable {
	public:
		Chat(Client &client);

		void setMessageVisibility(bool areMessagesVisible);

		const std::string &getHistoryEntry(u32 id) const { return m_history.at(m_history.size() - id - 1); }
		void addHistoryEntry(const std::string &entry) { m_history.emplace_back(entry); }
		u32 historySize() const { return m_history.size(); }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		std::deque<ChatMessage> m_chatMessages;
		std::deque<std::string> m_history;

		u32 m_posY = 0;
};

#endif // CHAT_HPP_
