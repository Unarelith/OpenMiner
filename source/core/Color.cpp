/*
 * =====================================================================================
 *
 *       Filename:  Color.cpp
 *
 *    Description:
 *
 *        Created:  27/09/2014 22:35:45
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#include "Color.hpp"

Color Color::black(0, 0, 0);
Color Color::white(255, 255, 255);
Color Color::blue(32, 168, 248);
Color Color::red(232, 8, 24);

Color::Color(u8 _r, u8 _g, u8 _b, u8 _a) {
	r = _r / 255.0f;
	g = _g / 255.0f;
	b = _b / 255.0f;
	a = _a / 255.0f;
}

Color Color::operator-(const Color &color) {
	return Color(r - color.r, g - color.g, b - color.b, a - color.a);
}

