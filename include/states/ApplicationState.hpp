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
		virtual ~ApplicationState() = default;

		virtual void handleEvents();

		virtual void update() = 0;

		virtual void draw() = 0;
};

#endif // APPLICATIONSTATE_HPP_
