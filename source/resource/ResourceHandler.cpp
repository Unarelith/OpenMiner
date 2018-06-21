/*
 * =====================================================================================
 *
 *       Filename:  ResourceHandler.cpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:30:47
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ResourceHandler.hpp"

ResourceHandler *ResourceHandler::instance = nullptr;

ResourceHandler &ResourceHandler::getInstance() {
	return *instance;
}

void ResourceHandler::setInstance(ResourceHandler &handler) {
	instance = &handler;
}

