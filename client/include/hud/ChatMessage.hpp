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
#ifndef CHATMESSAGE_HPP_
#define CHATMESSAGE_HPP_

#include "Text.hpp"

class ChatMessage : public gk::Drawable, public gk::Transformable {
	public:
		ChatMessage(u16 clientID, const std::string &message, u32 messageCount);

		void setVisible(bool isVisible) { m_isVisible = isVisible; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		Text m_text;

		gk::Timer m_timer;

		bool m_isVisible = false;
};

#endif // CHATMESSAGE_HPP_
