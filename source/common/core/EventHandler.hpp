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
#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_

#include <memory>
#include <mutex>
#include <unordered_map>
#include <typeindex>

#include "core/EventListenerList.hpp"

class EventHandler {
	public:
		template<typename T, typename F>
		void addListener(F &&func) {
			std::lock_guard<std::mutex> lock(m_mutex);
			getListenerList<T>().addListener(std::forward<F>(func));
		}

		template<typename T, typename F, typename I>
		void addListener(F &&func, I *instance) {
			std::lock_guard<std::mutex> lock(m_mutex);
			getListenerList<T>().addListener(std::bind(func, instance, std::placeholders::_1), instance);
		}

		void removeListeners(void *instance) {
			std::lock_guard<std::mutex> lock(m_mutex);
			for (auto &it : m_listenerLists)
				it.second->removeListeners(instance);
		}

		template<typename T>
		void pushEvent(const T &event) {
			std::lock_guard<std::mutex> lock(m_mutex);
			getListenerList<T>().pushEvent(event);
		}

		template<typename T, typename... Args>
		void emplaceEvent(Args &&...args) {
			std::lock_guard<std::mutex> lock(m_mutex);
			getListenerList<T>().pushEvent(T{std::forward<Args>(args)...});
		}

		void processEvents() {
			std::lock_guard<std::mutex> lock(m_mutex);
			for (auto &it : m_listenerLists)
				it.second->processEvents();
		}

	private:
		template<typename T>
		EventListenerList<T> &getListenerList() {
			auto it = m_listenerLists.find(typeid(T));
			if(it == m_listenerLists.end()) {
				m_listenerLists[typeid(T)] = std::make_shared<EventListenerList<T>>();
				return *std::static_pointer_cast<EventListenerList<T>>(m_listenerLists[typeid(T)]);
			}
			else {
				return *std::static_pointer_cast<EventListenerList<T>>(it->second);
			}
		}

		std::unordered_map<std::type_index, std::shared_ptr<IEventListenerList>> m_listenerLists;

		std::mutex m_mutex;
};

#endif // EVENTHANDLER_HPP_
