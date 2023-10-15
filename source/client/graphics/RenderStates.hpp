/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef RENDERSTATES_HPP_
#define RENDERSTATES_HPP_

#include <glm/matrix.hpp>

#include <gk/core/IntTypes.hpp>
#include <gk/gl/Transform.hpp>

class Shader;

namespace gk {
	class Texture;
}

struct RenderStates {
	gk::Transform projectionMatrix;
	gk::Transform viewMatrix;
	gk::Transform transform;

	const gk::Texture *texture = nullptr;
	const Shader *shader = nullptr;

	static const RenderStates Default; // Defined in RenderTarget.cpp
};

#endif // RENDERSTATES_HPP_
