/*
 * =====================================================================================
 *
 *       Filename:  ApplicationStateStack.hpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 13:48:48
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef APPLICATIONSTATESTACK_HPP_
#define APPLICATIONSTATESTACK_HPP_

#include <memory>
#include <stack>

#include "ApplicationState.hpp"

class ApplicationStateStack {
	public:
		ApplicationState &top() { return *m_stack.top().get(); }
		void push(ApplicationState *state) { m_stack.push(std::unique_ptr<ApplicationState>(state)); }
		void pop() { m_stack.pop(); }

		static ApplicationStateStack &getInstance() {
			static ApplicationStateStack instance;
			return instance;
		}

	private:
		std::stack<std::unique_ptr<ApplicationState>> m_stack;
};

#endif // APPLICATIONSTATESTACK_HPP_
