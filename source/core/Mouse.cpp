/*
 * =====================================================================================
 *
 *       Filename:  Mouse.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  20/12/2014 15:39:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
 *
 * =====================================================================================
 */
#include "Mouse.hpp"

s32 Mouse::x = 0;
s32 Mouse::y = 0;

s32 Mouse::dx = 0;
s32 Mouse::dy = 0;

void Mouse::reset() {
	x = 0;
	y = 0;

	dx = 0;
	dy = 0;
}

void Mouse::update(s32 x, s32 y, s32 dx, s32 dy) {
	Mouse::x = x;
	Mouse::y = y;

	Mouse::dx = dx;
	Mouse::dy = dy;
}

