/*
 * =====================================================================================
 *
 *       Filename:  Exception.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14/12/2014 05:02:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
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
			m_line = line;
			m_filename = filename;
			
			std::stringstream stream;
			m_errorMsg = Debug::makeString(stream, args...);
		}
		
		~Exception() throw() {
		}
		
		virtual std::string what() const throw() {
			return Debug::textColor(Debug::TextColor::Red, true) + "at " + m_filename + ":" + std::to_string(m_line) + ": " + Debug::textColor(0, true) + m_errorMsg.c_str() + Debug::textColor();
		}
		
	private:
		u16 m_line;
		std::string m_filename;
		
		std::string m_errorMsg;
};

#endif // EXCEPTION_HPP_
