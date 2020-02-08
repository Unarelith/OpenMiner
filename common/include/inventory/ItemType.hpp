/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef ITEMTYPE_HPP_
#define ITEMTYPE_HPP_

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
		Glowstone    = 12,
		Workbench    = 13,
		Furnace      = 14,
		IronOre      = 15,
		PlankSlab    = 16,

		Stick         = 32,
		StoneAxe      = 33,
		StoneHoe      = 34,
		StonePickaxe  = 35,
		StoneShovel   = 36,
		StoneSword    = 37,
		Coal          = 38,
		IronIngot     = 39,
		Charcoal      = 40,
		WoodenAxe     = 41,
		WoodenHoe     = 42,
		WoodenPickaxe = 43,
		WoodenShovel  = 44,
		WoodenSword   = 45,
	};
}

#endif // ITEMTYPE_HPP_
