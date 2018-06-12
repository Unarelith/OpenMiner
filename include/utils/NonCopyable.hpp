/*
 * =====================================================================================
 *
 *       Filename:  NonCopyable.hpp
 *
 *    Description:
 *
 *        Created:  12/06/2018 22:23:46
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef NONCOPYABLE_HPP_
#define NONCOPYABLE_HPP_

class NonCopyable {
	protected:
		NonCopyable() = default;
		NonCopyable(const NonCopyable &) = delete;

		const NonCopyable &operator=(const NonCopyable &) = delete;
};

#endif // NONCOPYABLE_HPP_
