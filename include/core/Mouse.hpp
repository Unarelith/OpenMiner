/*
 * =====================================================================================
 *
 *       Filename:  Mouse.hpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  20/12/2014 15:38:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
 *
 * =====================================================================================
 */
#ifndef MOUSE_HPP_
#define MOUSE_HPP_

#include "Types.hpp"

class Mouse {
	public:
		static void reset();
		static void update(s32 x, s32 y, s32 dx, s32 dy);

		static s32 getX() { return x; }
		static s32 getY() { return y; }

		static s32 getDX() { return dx; }
		static s32 getDY() { return dy; }

	private:
		static s32 x;
		static s32 y;

		static s32 dx;
		static s32 dy;
};

#endif // MOUSE_HPP_
