/*
 * =====================================================================================
 *
 *       Filename:  GamePad.cpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:12:09
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "GamePad.hpp"

InputHandler *GamePad::inputHandler = nullptr;

bool GamePad::isKeyPressed(GameKey key) {
	return (inputHandler) ? inputHandler->isKeyPressed(key) : false;
}

bool GamePad::isKeyPressedOnce(GameKey key) {
	return (inputHandler) ? inputHandler->isKeyPressedOnce(key) : false;
}

bool GamePad::isKeyPressedWithDelay(GameKey key, u16 delay) {
	return (inputHandler) ? inputHandler->isKeyPressedWithDelay(key, delay) : false;
}

