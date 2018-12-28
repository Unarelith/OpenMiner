/*
 * =====================================================================================
 *
 *       Filename:  Item.hpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:29:41
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <string>

#include "IntTypes.hpp"
#include "ItemType.hpp"

class Item {
	public:
		Item(u32 id, u32 textureID, const std::string &name, const std::string &label);

		const std::string &name() const { return m_name; }
		const std::string &label() const { return m_label; }

		u32 id() const { return m_id; }
		u32 textureID() const { return m_textureID; }

		bool isBlock() const { return m_isBlock; }
		void setIsBlock(bool isBlock) { m_isBlock = isBlock; }

		bool isFuel() const { return m_isFuel; }
		void setIsFuel(bool isFuel) { m_isFuel = isFuel; }

		u16 burnTime() const { return m_burnTime; }
		void setBurnTime(u16 burnTime) { m_burnTime = burnTime; }

		u8 harvestCapability() const { return m_harvestCapability; }
		void setHarvestCapability(u8 harvestCapability) { m_harvestCapability = harvestCapability; }

		float miningSpeed() const { return m_miningSpeed; }
		void setMiningSpeed(float miningSpeed) { m_miningSpeed = miningSpeed; }

	protected:
		bool m_isBlock = false;
		bool m_isFuel = false;

		u16 m_burnTime = 0;

	private:
		u32 m_id = 0;
		u32 m_textureID = 0;

		std::string m_name;
		std::string m_label;

		u8 m_harvestCapability = 0;
		float m_miningSpeed = 1;
};

#endif // ITEM_HPP_
