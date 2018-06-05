/*
 * =====================================================================================
 *
 *       Filename:  ApplicationState.hpp
 *
 *    Description:
 *
 *        Created:  05/06/2018 15:45:26
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef APPLICATIONSTATE_HPP_
#define APPLICATIONSTATE_HPP_

class ApplicationState {
	public:
		ApplicationState(ApplicationState *parent = nullptr) : m_parent(parent) {}
		virtual ~ApplicationState() = default;

		virtual void update() = 0;

		virtual void draw() = 0;

	protected:
		ApplicationState *m_parent = nullptr;
};

#endif // APPLICATIONSTATE_HPP_
