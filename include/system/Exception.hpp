/*
 * =====================================================================================
 *
 *       Filename:  Exception.hpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 05:02:53
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>

#include "Debug.hpp"
#include "Types.hpp"

#define EXCEPTION(args...) (Exception(__LINE__, _FILE, args))

class Exception {
	public:
		template<typename... Args>
		Exception(u16 line, std::string filename, Args... args) throw() {
			m_errorMsg = Debug::textColor(Debug::TextColor::Red, true) + "at " + filename + ":" + std::to_string(line) + ": " + Debug::textColor(0, true) + Debug::makeString(args...) + Debug::textColor();
		}

		virtual std::string what() const noexcept {
			return m_errorMsg.c_str();
		}

	private:
		std::string m_errorMsg;
};

#endif // EXCEPTION_HPP_
