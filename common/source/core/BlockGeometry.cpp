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
#include <mutex>

#include "BlockGeometry.hpp"

namespace BlockGeometry {

glm::mat3 orientMatrices[nOrientations];

static void doInitOrientation(void);

void initOrientation() {
	static int orientInitialized = false;
	static std::mutex orientInitMutex;
	std::lock_guard<std::mutex> lock(orientInitMutex);

	if (!orientInitialized) {
		doInitOrientation();
		orientInitialized = true;
	}
}

static void doInitOrientation() {
	// Build all 24 rotation matrices for the orientation field

	// Local rotation of top face
	glm::mat3 topRotation{
	    1, 0, 0,
	    0, 1, 0,
	    0, 0, 1,
	};
	// Matrix for a rotation of 90 degrees CCW around the Z axis, used to
	// rotate the top face in 90 degree steps around its local Z axis.
	const glm::mat3 rotate90Z{
		// Note glm matrices are column-major, so each row here is
		// actually a column of the matrix.
		 0, 1, 0,
		-1, 0, 0,
		 0, 0, 1,
	};
	// Rotations that place the top face on each of the axes
	const glm::mat3 rot2axis[nAxes]{
		{ 1,  0,  0,   0,  1,  0,   0,  0,  1},  // top face on +Z, no rotation (identity matrix)
		{ 1,  0,  0,   0, -1,  0,   0,  0, -1},  // top face on -Z, rotating around the X axis
		{ 1,  0,  0,   0,  0, -1,   0,  1,  0},  // top face on +Y, rotating around the X axis
		{ 1,  0,  0,   0,  0,  1,   0, -1,  0},  // top face on -Y, rotating around the X axis
		{ 0,  0,  1,   0,  1,  0,  -1,  0,  0},  // top face on +X, rotating around the Y axis
		{ 0,  0, -1,   0,  1,  0,   1,  0,  0},  // top face on -X, rotating around the Y axis
	};

	for (int axis = 0; axis < nAxes; ++axis) {
		for (int angle = 0; angle < nRots; ++angle) {
			// Rotate the top around itself, then rotate it to each face
			orientMatrices[axis * 4 + angle] = rot2axis[axis] * topRotation;

			// Next top rotation
			topRotation = rotate90Z * topRotation;
		}
	}

	// Since we use bit masking, we add eight dummy orientations to the normal
	// set of 24 to complete a power of two, to make the code robust against
	// invalid orientation values.
	for (int axis = nAxes; axis < nAxesP2; ++axis) {
		for (int angle = 0; angle < nRots; ++angle) {
			orientMatrices[axis * nRots + angle] = orientMatrices[0];
		}
	}
}

} // namespace BlockGeometry
