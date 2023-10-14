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
#ifndef APPLICATIONSTATESTACK_HPP_
#define APPLICATIONSTATESTACK_HPP_

#include <memory>
#include <stack>

#include "ApplicationState.hpp"

namespace gk {
	class EventHandler;
}

class ApplicationStateStack {
	public:
		template<typename T, typename... Args>
		auto push(Args &&...args) -> typename std::enable_if<std::is_base_of<ApplicationState, T>::value, T&>::type {
			if (!empty())
				m_states.top()->onStateInactive();

			m_states.emplace(std::make_shared<T>(std::forward<Args>(args)...));

			m_states.top()->setStateStack(this);
			m_states.top()->setEventHandler(m_eventHandler);
			m_states.top()->init();

			return static_cast<T&>(top());
		}

		void pop();

		void clear() { while(!empty()) pop(); }

		void clearDeletedStates();

		ApplicationState &top() const { return *m_states.top().get(); }

		bool empty() const { return m_states.empty(); }

		std::size_t size() const { return m_states.size(); }

		void setEventHandler(gk::EventHandler &eventHandler) {
			m_eventHandler = &eventHandler;
		}

		static ApplicationStateStack &getInstance() {
			return *s_instance;
		}

		static void setInstance(ApplicationStateStack &instance) {
			s_instance = &instance;
		}

	private:
		static ApplicationStateStack *s_instance;

		std::stack<std::shared_ptr<ApplicationState>> m_states;
		std::stack<std::shared_ptr<ApplicationState>> m_trash;

		gk::EventHandler *m_eventHandler = nullptr;
};


#endif // APPLICATIONSTATESTACK_HPP_
