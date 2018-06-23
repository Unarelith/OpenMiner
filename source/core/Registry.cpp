/*
 * =====================================================================================
 *
 *       Filename:  Registry.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:43:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "BlockWater.hpp"
#include "Registry.hpp"

Registry *Registry::s_instance = nullptr;

void Registry::registerBlocks() {
	registerBlock<Block>(0);
	registerBlock<Block>(1);
	registerBlock<Block>(2);
	registerBlock<Block>(3);
	registerBlock<Block>(4);
	registerBlock<Block>(5);
	registerBlock<Block>(6);
	registerBlock<Block>(7);
	registerBlock<BlockWater>(8);
	registerBlock<Block>(9);
	registerBlock<Block>(10);
	registerBlock<Block>(11);
	registerBlock<Block>(12);
	registerBlock<Block>(13);
	registerBlock<Block>(14);
	registerBlock<Block>(15);
	registerBlock<Block>(16);
	registerBlock<Block>(17);
	registerBlock<Block>(18);
	registerBlock<Block>(19);
	registerBlock<Block>(20);
	registerBlock<Block>(21);
	registerBlock<Block>(22);
	registerBlock<Block>(23);
	registerBlock<Block>(24);
}

void Registry::registerItems() {
	registerItem<Item>(0);
	registerItem<Item>(1);
	registerItem<Item>(2);
	registerItem<Item>(3);
	registerItem<Item>(4);
	registerItem<Item>(5);
	registerItem<Item>(6);
	registerItem<Item>(7);
	registerItem<Item>(8);
	registerItem<Item>(9);
	registerItem<Item>(10);
	registerItem<Item>(11);
	registerItem<Item>(12);
	registerItem<Item>(13);
	registerItem<Item>(14);
	registerItem<Item>(15);
	registerItem<Item>(16);
	registerItem<Item>(17);
	registerItem<Item>(18);
	registerItem<Item>(19);
	registerItem<Item>(20);
	registerItem<Item>(21);
	registerItem<Item>(22);
	registerItem<Item>(23);
	registerItem<Item>(24);
}

