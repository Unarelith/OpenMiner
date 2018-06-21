/*
 * =====================================================================================
 *
 *       Filename:  Debug.hpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 05:02:30
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Types.hpp"

#define DEBUG_ENABLED
#define DEBUG_COLOR

#define _FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef DEBUG_ENABLED
	#define DEBUG(args...) { std::cout << Debug::textColor(Debug::TextColor::Red, true) << _FILE << ":" << __LINE__ << ":" << Debug::textColor(); Debug::print(args); }
#else
	#define DEBUG(args...) {}
#endif

namespace Debug {
	enum TextColor {
		White = 0,
		Red = 31,
		Blue = 36
	};

	inline std::string textColor(u8 color = TextColor::White, bool bold = false) {
#ifdef DEBUG_COLOR
		return std::string("\33[0;") + ((color < 10) ? "0" : "") + std::to_string(color) + ";0" + ((bold) ? "1" : "0") + "m";
#else
		return std::string("");
#endif
	}

	template<typename... Args>
	std::string makeString(Args &&...args) {
		std::ostringstream stream;
		std::vector<int> tmp{0, ((void)(stream << args << " "), 0)...};

		return stream.str();
	}

	template<typename... Args>
	void print(Args &&...args) {
		std::cerr << makeString(std::forward<Args>(args)...) << std::endl;
	}
}

#endif // DEBUG_HPP_
