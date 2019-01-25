/*
 * =====================================================================================
 *
 *       Filename:  World.cpp
 *
 *    Description:
 *
 *        Created:  16/12/2014 15:28:19
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include <gk/resource/ResourceHandler.hpp>

#include "Config.hpp"
#include "World.hpp"

bool World::isReloadRequested = false;

