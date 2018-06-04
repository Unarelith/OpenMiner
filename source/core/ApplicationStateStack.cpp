/*
 * =====================================================================================
 *
 *       Filename:  ApplicationStateStack.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  14/12/2014 13:49:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
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

