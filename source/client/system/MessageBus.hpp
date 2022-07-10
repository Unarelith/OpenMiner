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
#ifndef MESSAGEBUS_HPP_
#define MESSAGEBUS_HPP_

#include <entt/entt.hpp>

class MessageBus {
	public:
		template<typename EventType, auto callback, typename Listener>
		void subscribe(Listener &listener) {
			m_dispatcher.sink<EventType>().template connect<callback>(listener);
		}

		template<typename EventType>
		void publish(const EventType &event) {
			m_dispatcher.enqueue<EventType>(event);
		}

		template<typename EventType, typename... Args>
		void publish(Args &&...args) {
			m_dispatcher.enqueue<EventType>(std::forward<Args>(args)...);
		}

		void update() {
			m_dispatcher.update();
		}

	private:
		entt::dispatcher m_dispatcher;
};

#endif // MESSAGEBUS_HPP_
