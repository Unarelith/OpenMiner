/*
 * =====================================================================================
 *
 *       Filename:  ItemType.hpp
 *
 *    Description:
 *
 *        Created:  25/06/2018 17:36:47
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ITEMTYPE_HPP_
#define ITEMTYPE_HPP_

#include "Types.hpp"

namespace ItemType {
	enum {
		Air          = 0,
		Dirt         = 1,
		Cobblestone  = 2,
		Grass        = 3,
		Leaves       = 4,
		Wood         = 5,
		Stone        = 6,
		Sand         = 7,
		Water        = 8,
		Glass        = 9,
		CoalOre      = 10,
		Planks       = 11,

		Stick        = 12,
		StoneAxe     = 13,
		StoneHoe     = 14,
		StonePickaxe = 15,
		StoneShovel  = 16,
		StoneSword   = 17,
	};
}

#endif // ITEMTYPE_HPP_
