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
#ifndef APPLICATIONSTATE_HPP_
#define APPLICATIONSTATE_HPP_

#include <gk/core/EventHandler.hpp>
#include <gk/core/SDLHeaders.hpp>
#include <gk/gl/Transformable.hpp>
#include <gk/utils/NonCopyable.hpp>

class ApplicationStateStack;

class ApplicationState : public gk::Transformable, public gk::NonCopyable {
	public:
		ApplicationState(ApplicationState *parent = nullptr) : m_parent(parent) {}
		ApplicationState(ApplicationState &&) = default;
		virtual ~ApplicationState() {
			if (m_eventHandler)
				m_eventHandler->removeListeners(this);
		}

		ApplicationState &operator=(ApplicationState &&) = default;

		virtual void init() {}

		virtual void onEvent(const SDL_Event &) {}

		virtual void update() {}

		ApplicationState *parent() { return m_parent; }

		void setStateStack(ApplicationStateStack *stateStack) { m_stateStack = stateStack; }

		void setEventHandler(gk::EventHandler *eventHandler) { m_eventHandler = eventHandler; }

		virtual void onStateInactive() {}

	protected:
		ApplicationState *m_parent = nullptr;

		ApplicationStateStack *m_stateStack = nullptr;

		gk::EventHandler *m_eventHandler = nullptr;
};

#endif // APPLICATIONSTATE_HPP_
