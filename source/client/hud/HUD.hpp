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
#ifndef HUD_HPP_
#define HUD_HPP_

#include <gk/gl/View.hpp>

#include "BlockCursor.hpp"
#include "BlockInfoWidget.hpp"
#include "Chat.hpp"
#include "Crosshair.hpp"
#include "DebugLightmapViewer.hpp"
#include "DebugOverlay.hpp"
#include "DebugProfilerWindow.hpp"
#include "Hotbar.hpp"
#include "Minimap.hpp"
#include "Shader.hpp"

struct GuiScaleChangedEvent;

class HUD : public gk::Transformable, public Drawable {
	public:
		HUD(ClientPlayer &player, ClientWorld &world, ClientCommandHandler &client);

		void setup();

		void onEvent(const SDL_Event &event);
		void onGuiScaleChanged(const GuiScaleChangedEvent &event);

		void update();

		void pause() { m_blockCursor.reset(); }

		const BlockCursor &blockCursor() const { return m_blockCursor; }

		Chat &chat() { return m_chat; }

		Minimap &minimap() { return m_minimap; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		ClientPlayer &m_player;
		ClientWorld &m_world;

		Shader m_shader;
		gk::View m_view;

		Hotbar m_hotbar;

		BlockCursor m_blockCursor;
		Crosshair m_crosshair;

		DebugOverlay m_debugOverlay;
		bool m_isDebugOverlayVisible = false;
		bool m_printOpenGLInfo = false;

		BlockInfoWidget m_blockInfoWidget;

		Text m_fpsText;

		Chat m_chat;

		Minimap m_minimap;

		DebugLightmapViewer m_debugLightmapViewer;

		DebugProfilerWindow m_debugProfilerWindow;
};

#endif // HUD_HPP_
