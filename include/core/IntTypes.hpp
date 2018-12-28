/*
 * =====================================================================================
 *
 *       Filename:  IntTypes.hpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 05:05:33
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef INTTYPES_HPP_
#define INTTYPES_HPP_

using s8  = signed char;
using s16 = signed short;
using s32 = signed int;

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;

#if defined(_MSC_VER)
	using s64 = signed __int64;
	using u64 = unsigned __int64;
#else
	using s64 = signed long long;
	using u64 = unsigned long long;
#endif

#endif // INTTYPES_HPP_
