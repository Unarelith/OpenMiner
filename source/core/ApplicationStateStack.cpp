/*
 * =====================================================================================
 *
 *       Filename:  ApplicationStateStack.cpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 13:49:05
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ApplicationStateStack.hpp"
#include "GameState.hpp"

ApplicationStateStack::ApplicationStateStack() {
	push(new GameState);
}

ApplicationStateStack::~ApplicationStateStack() {
}

ApplicationStateStack &ApplicationStateStack::getInstance() {
	static ApplicationStateStack instance;
	return instance;
}

