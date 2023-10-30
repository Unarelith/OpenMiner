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
#include "Config.hpp"
#include "Crosshair.hpp"

Crosshair::Crosshair() {
	setup();
}

void Crosshair::setup() {
	m_hShape.setSize(41, 4);
	m_vShape1.setSize(4, 19);
	m_vShape2.setSize(4, 19);

	m_hShape.setFillColor(Color::fromRGBA32(200, 200, 200, 180));
	m_vShape1.setFillColor(Color::fromRGBA32(200, 200, 200, 180));
	m_vShape2.setFillColor(Color::fromRGBA32(200, 200, 200, 180));

	m_hShape.setPosition(Config::screenWidth / 2.0f - m_hShape.width() / 2, Config::screenHeight / 2.0f - m_hShape.height() / 2, 0);
	m_vShape1.setPosition(Config::screenWidth / 2.0f - m_vShape1.width() / 2, Config::screenHeight / 2.0f - m_hShape.height() / 2 - m_vShape1.height(), 0);
	m_vShape2.setPosition(Config::screenWidth / 2.0f - m_vShape2.width() / 2, Config::screenHeight / 2.0f + m_hShape.height() / 2, 0);
}

void Crosshair::draw(RenderTarget &target, RenderStates states) const {
	target.draw(m_hShape, states);
	target.draw(m_vShape1, states);
	target.draw(m_vShape2, states);
}

