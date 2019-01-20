/*
 * =====================================================================================
 *
 *       Filename:  Recipe.hpp
 *
 *    Description:
 *
 *        Created:  30/06/2018 03:01:44
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef RECIPE_HPP_
#define RECIPE_HPP_

#include "Inventory.hpp"

class Recipe {
	public:
		Recipe(const std::string &type, const ItemStack &result) : m_type(type), m_result(result) {}
		virtual ~Recipe() = default;

		virtual bool isMatching(const Inventory &inventory) const = 0;

		const std::string &type() const { return m_type; }

		const ItemStack &result() const { return m_result; }

	protected:
		std::string m_type;

		ItemStack m_result;
};

#endif // RECIPE_HPP_
