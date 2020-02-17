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
#include "BlockCursorRaycast.hpp"
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
                             const Axis axis,
                             const bool neg,
                             const int_fast32_t nx,
                             const int_fast32_t ny,
                             const int_fast32_t nz,
                             int_fast32_t &bestX,
                             int_fast32_t &bestY,
                             int_fast32_t &bestZ,
                             int_fast8_t &bestFace,
                             double &bestDepth,
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
                                       const glm::dvec3 &lookAt,
                                       const double maxReach,
                                       int_fast32_t &bestX, int_fast32_t &bestY,
                                       int_fast32_t &bestZ,
                                       int_fast8_t &bestFace, double &bestDepth,
                                       const ClientWorld &world)
{
	glm::dvec3 isect;
	double posCoord, lookAtCoord;

	// The line that starts at 'position' and goes in the direction
	// of 'lookAt' with length 'maxReach' crosses several nodes in
	// the X direction at integer positions. Determine the first and
	// last such positions.
	switch(axis) {
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

	int_fast32_t firstNodeRow = lookAtCoord < 0. ? int_fast32_t(floor(posCoord)) : int_fast32_t(ceil(posCoord)) - 1;
	int_fast32_t lastNodeRow = int_fast32_t(floor(posCoord + lookAtCoord * maxReach));

	int_fast8_t dir = (lookAtCoord > 0.) - (lookAtCoord < 0.);
	if (!dir) {
		// Can't cross any planes if it doesn't change in this axis
		return;
	}

	for(int_fast32_t nodeRow = firstNodeRow + dir;
			dir > 0 ? (nodeRow <= lastNodeRow) : (nodeRow >= lastNodeRow); nodeRow += dir)
	{
		isect = intersectAxisPlane(axis, double(nodeRow + (dir < 0)), position, lookAt);

		int_fast32_t nx, ny, nz;
		nx = axis == AXIS_X ? nodeRow : floor(isect.x);
		ny = axis == AXIS_Y ? nodeRow : floor(isect.y);
		nz = axis == AXIS_Z ? nodeRow : floor(isect.z);

		u32 blockID = world.getBlock(nx, ny, nz);
		const Block &block = Registry::getInstance().getBlock(blockID);
		if(blockID && block.drawType() != BlockDrawType::Liquid) {
			// Check bounding box; this should loop over all selection boxes
			// when they are implemented
			gk::FloatBox selBox = block.boundingBox();
			selBox.x += double(nx);
			selBox.y += double(ny);
			selBox.z += double(nz);

			bool hit = false;

			// Check if we hit any of the sides of the inner box
			isect = intersectAxisPlane(AXIS_X, (lookAt.x < 0. ? selBox.x + selBox.width : selBox.x), position, lookAt);
			if (selBox.y <= isect.y && isect.y <= selBox.y + selBox.height
			 && selBox.z <= isect.z && isect.z <= selBox.z + selBox.depth)
				recordHit(position, isect, AXIS_X, lookAt.x < 0., nx, ny, nz, bestX, bestY, bestZ, bestFace, bestDepth, hit);

			isect = intersectAxisPlane(AXIS_Y, (lookAt.y < 0. ? selBox.y + selBox.height : selBox.y), position, lookAt);
			if (selBox.x <= isect.x && isect.x <= selBox.x + selBox.width
			 && selBox.z <= isect.z && isect.z <= selBox.z + selBox.depth)
				recordHit(position, isect, AXIS_Y, lookAt.y < 0., nx, ny, nz, bestX, bestY, bestZ, bestFace, bestDepth, hit);

			isect = intersectAxisPlane(AXIS_Z, (lookAt.z < 0. ? selBox.z + selBox.depth : selBox.z), position, lookAt);
			if (selBox.x <= isect.x && isect.x <= selBox.x + selBox.width
			 && selBox.y <= isect.y && isect.y <= selBox.y + selBox.height)
				recordHit(position, isect, AXIS_Z, lookAt.z < 0., nx, ny, nz, bestX, bestY, bestZ, bestFace, bestDepth, hit);

			if (hit)
				break;  // no need to check deeper
		}
	}
}

