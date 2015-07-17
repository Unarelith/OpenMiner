/*
 * =====================================================================================
 *
 *       Filename:  ApplicationState.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14/12/2014 13:49:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef APPLICATIONSTATE_HPP_
#define APPLICATIONSTATE_HPP_

class ApplicationState {
	public:
		ApplicationState(ApplicationState *parent = nullptr) : m_parent(parent) {}
		ApplicationState(const ApplicationState &) = delete;
		ApplicationState(ApplicationState &&) = default;
		virtual ~ApplicationState() = default;
		
		virtual void update() = 0;
		
		virtual void draw() = 0;
		
	protected:
		ApplicationState *m_parent = nullptr;
};

#endif // APPLICATIONSTATE_HPP_
