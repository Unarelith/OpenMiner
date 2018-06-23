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
	registerBlock<Block>(8);
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
}

void Registry::registerItems() {
}

