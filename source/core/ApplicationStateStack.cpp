/*
 * =====================================================================================
 *
 *       Filename:  ApplicationStateStack.cpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 01:58:34
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ApplicationStateStack.hpp"

ApplicationStateStack *ApplicationStateStack::s_instance = nullptr;

void ApplicationStateStack::pop() {
	m_trash.push(m_states.top());
	m_states.pop();
}

void ApplicationStateStack::clearDeletedStates() {
	while (!m_trash.empty())
		m_trash.pop();
}

