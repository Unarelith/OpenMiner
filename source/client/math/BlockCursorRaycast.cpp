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
#include "BlockCursorRaycast.hpp"
#include "BlockGeometry.hpp"
#include "ClientWorld.hpp"
#include "Registry.hpp"

static inline glm::dvec3 intersectAxisPlane(const Axis axis, const double coord,
                                            const glm::dvec3 &position,
                                            const glm::dvec3 &direction)
{
	const double t = axis == AXIS_X ? (coord - position.x) / direction.x
	               : axis == AXIS_Y ? (coord - position.y) / direction.y
	                                : (coord - position.z) / direction.z;

	glm::dvec3 result{
		axis == AXIS_X ? coord : position.x + t * direction.x,
		axis == AXIS_Y ? coord : position.y + t * direction.y,
		axis == AXIS_Z ? coord : position.z + t * direction.z
	};

	return result;
}

static inline void recordHit(const glm::dvec3 &position,
                             const glm::dvec3 &isect,
                             const Axis axis, const bool neg,
                             const s32f nx, const s32f ny, const s32f nz,
                             s32f &bestX, s32f &bestY, s32f &bestZ,
                             s8f &bestFace, double &bestDepth,
                             bool &hit)
{
	// Check if we have a record
	double depth = glm::distance(position, isect);
	if (bestFace == -1 || depth < bestDepth) {
		// Yep, register it
		bestFace = axis == AXIS_X ? (neg ? 0 : 3)
		         : axis == AXIS_Y ? (neg ? 1 : 4)
		                          : (neg ? 2 : 5);
		bestDepth = depth;
		bestX = nx;
		bestY = ny;
		bestZ = nz;
	}

	hit = true;
}

void BlockCursorRaycast::rayCastToAxis(const Axis axis, const glm::dvec3 &position,
                                       const glm::dvec3 &lookAt, const double maxReach,
                                       s32f &bestX, s32f &bestY, s32f &bestZ,
                                       s8f &bestFace, double &bestDepth,
                                       const ClientWorld &world)
{
	glm::dvec3 isect;
	double posCoord, lookAtCoord;

	// The line that starts at 'position' and goes in the direction
	// of 'lookAt' with length 'maxReach' crosses several nodes in
	// the X direction at integer positions. Determine the first and
	// last such positions.
	switch (axis) {
		case AXIS_X:
			posCoord = position.x;
			lookAtCoord = lookAt.x;
			break;
		case AXIS_Y:
			posCoord = position.y;
			lookAtCoord = lookAt.y;
			break;
		default:
			posCoord = position.z;
			lookAtCoord = lookAt.z;
			break;
	}

	s32f firstNodeRow = lookAtCoord < 0. ? s32f(floor(posCoord)) : s32f(ceil(posCoord)) - 1;
	s32f lastNodeRow = s32f(floor(posCoord + lookAtCoord * maxReach));

	s8f dir = s8f((lookAtCoord > 0.) - (lookAtCoord < 0.));
	if (!dir) {
		// Can't cross any planes if it doesn't change in this axis
		return;
	}

	for (s32f nodeRow = firstNodeRow + dir;
	    dir > 0 ? (nodeRow <= lastNodeRow) : (nodeRow >= lastNodeRow); nodeRow += dir)
	{
		isect = intersectAxisPlane(axis, double(nodeRow + (dir < 0)), position, lookAt);

		s32f nx, ny, nz;
		nx = axis == AXIS_X ? nodeRow : (s32f)floor(isect.x);
		ny = axis == AXIS_Y ? nodeRow : (s32f)floor(isect.y);
		nz = axis == AXIS_Z ? nodeRow : (s32f)floor(isect.z);

		const BlockState *blockState = world.getBlockState((int)nx, (int)ny, (int)nz);
		if (!blockState) continue;

		u8f orientation = blockState->block().isRotatable() ? world.getData((int)nx, (int)ny, (int)nz) & 0x1F : 0;

		const FloatBox &boundingBox = blockState->boundingBox();
		glm::vec3 localCorner1{boundingBox.x, boundingBox.y, boundingBox.z};
		glm::vec3 localCorner2{boundingBox.sizeX, boundingBox.sizeY, boundingBox.sizeZ};
		localCorner2 += localCorner1;

		if (orientation) {
			const glm::vec3 half{0.5, 0.5, 0.5};
			localCorner1 = BlockGeometry::orientMatrices[orientation] * (localCorner1 - half) + half;
			localCorner2 = BlockGeometry::orientMatrices[orientation] * (localCorner2 - half) + half;
			if (localCorner2.x < localCorner1.x) std::swap(localCorner1.x, localCorner2.x);
			if (localCorner2.y < localCorner1.y) std::swap(localCorner1.y, localCorner2.y);
			if (localCorner2.z < localCorner1.z) std::swap(localCorner1.z, localCorner2.z);
		}

		if (blockState->block().id() && blockState->drawType() != BlockDrawType::Liquid) {
			// Check bounding box; this should loop over all selection boxes
			// when they are implemented
			const glm::dvec3 cubePos{double(nx), double(ny), double(nz)};
			glm::dvec3 corner1 = glm::dvec3(localCorner1) + cubePos;
			glm::dvec3 corner2 = glm::dvec3(localCorner2) + cubePos;

			bool hit = false;

			// Check if we hit any of the sides of the inner box
			isect = intersectAxisPlane(AXIS_X, (lookAt.x < 0. ? corner2.x : corner1.x), position, lookAt);
			if (corner1.y <= isect.y && isect.y <= corner2.y
			 && corner1.z <= isect.z && isect.z <= corner2.z)
				recordHit(position, isect, AXIS_X, lookAt.x < 0., nx, ny, nz, bestX, bestY, bestZ, bestFace, bestDepth, hit);

			isect = intersectAxisPlane(AXIS_Y, (lookAt.y < 0. ? corner2.y : corner1.y), position, lookAt);
			if (corner1.x <= isect.x && isect.x <= corner2.x
			 && corner1.z <= isect.z && isect.z <= corner2.z)
				recordHit(position, isect, AXIS_Y, lookAt.y < 0., nx, ny, nz, bestX, bestY, bestZ, bestFace, bestDepth, hit);

			isect = intersectAxisPlane(AXIS_Z, (lookAt.z < 0. ? corner2.z : corner1.z), position, lookAt);
			if (corner1.x <= isect.x && isect.x <= corner2.x
			 && corner1.y <= isect.y && isect.y <= corner2.y)
				recordHit(position, isect, AXIS_Z, lookAt.z < 0., nx, ny, nz, bestX, bestY, bestZ, bestFace, bestDepth, hit);

			if (hit)
				break;  // no need to check deeper
		}
	}
}
