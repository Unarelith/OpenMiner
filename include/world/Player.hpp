/*
 * =====================================================================================
 *
 *       Filename:  Player.hpp
 *
 *    Description:
 *
 *        Created:  27/06/2018 04:56:19
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Inventory.hpp"

class Player {
	public:
		Inventory &inventory() { return m_inventory; }

	private:
		Inventory m_inventory{9, 4};
};

#endif // PLAYER_HPP_
