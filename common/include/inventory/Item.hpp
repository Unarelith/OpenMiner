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

#include <gk/core/IntTypes.hpp>

#include "ItemType.hpp"
#include "ISerializable.hpp"
#include "TilesDef.hpp"

class Item : public ISerializable {
	public:
		Item() = default;
		Item(u32 id, const TilesDef &tiles, const std::string &name, const std::string &label);

		void serialize(sf::Packet &packet) override;
		void deserialize(sf::Packet &packet) override;

		const std::string &name() const { return m_name; }
		const std::string &label() const { return m_label; }

		std::string modName() const { return m_name.substr(0, m_name.find_first_of(":")); }

		u32 id() const { return m_id; }
		const TilesDef &tiles() const { return m_tiles; }

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
		TilesDef m_tiles;

		std::string m_name;
		std::string m_label;

		u8 m_harvestCapability = 0;
		float m_miningSpeed = 1;
};

#endif // ITEM_HPP_
