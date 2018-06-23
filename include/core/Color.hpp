/*
 * =====================================================================================
 *
 *       Filename:  Color.hpp
 *
 *    Description:
 *
 *        Created:  27/09/2014 22:35:38
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#ifndef COLOR_HPP_
#define COLOR_HPP_

#include "Types.hpp"

class Color {
	public:
		Color() = default;
		Color(u8 _r, u8 _g, u8 _b, u8 _a = 255);

		Color operator-(const Color &color);

		bool operator==(const Color &color) {
			return r == color.r && g == color.g && b == color.b && a == color.a;
		}

		bool operator!=(const Color &color) {
			return !(*this == color);
		}

		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;

		static Color black;
		static Color white;
		static Color blue;
		static Color red;
};

#endif // COLOR_HPP_
