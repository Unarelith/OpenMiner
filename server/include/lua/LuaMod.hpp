/*
 * =====================================================================================
 *
 *       Filename:  LuaMod.hpp
 *
 *    Description:
 *
 *        Created:  04/01/2019 17:19:44
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef LUAMOD_HPP_
#define LUAMOD_HPP_

#include <sol.hpp>

// This class is meant to be used ONLY in Lua
class LuaMod {
	public:
		LuaMod(const std::string &id) : m_id(id) {}

		void registerBlock(const sol::table &table);
		void registerItem(const sol::table &table);
		void registerCraftingRecipe(const sol::table &table);
		void registerSmeltingRecipe(const sol::table &table);

		const std::string &id() const { return m_id; }

		static void initUsertype(sol::state &lua);

	private:
		std::string m_id;
};

#endif // LUAMOD_HPP_
