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
#ifndef CLIENTEVENTS_HPP_
#define CLIENTEVENTS_HPP_

#include <vector>

#include "gk/core/IntTypes.hpp"

namespace gk {
	class ApplicationStateStack;
	class ApplicationState;
	class Drawable;
}

namespace GameplayEvent {
	struct RotateCamera {
		int x, y;
		int xrel, yrel;
	};

	struct PauseGame {
		gk::ApplicationStateStack *stateStack;
		gk::ApplicationState *currentState;
	};

	struct OpenChat {
		bool addSlash;

		gk::ApplicationStateStack *stateStack;
		gk::ApplicationState *currentState;
	};

	struct TakeScreenshot {};

	struct GameUpdate {
		bool allowWorldReload;
		bool processPlayerInputs;
		bool sendPlayerPosRotUpdate;
	};
}

namespace RenderingEvent {
	struct DrawObjects {
		std::vector<const gk::Drawable *> objects;
		bool inFramebuffer;
	};

	struct WindowSizeChanged {
		u16 width;
		u16 height;
	};
}


#endif // CLIENTEVENTS_HPP_
