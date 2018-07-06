/*
 * =====================================================================================
 *
 *       Filename:  DebugOverlay.cpp
 *
 *    Description:
 *
 *        Created:  06/07/2018 13:54:25
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <sstream>

#include "Camera.hpp"
#include "Config.hpp"
#include "DebugOverlay.hpp"

DebugOverlay::DebugOverlay(const Camera &camera) : m_camera(camera) {
	setPosition(4, 4, 0);
	setScale(GUI_SCALE, GUI_SCALE, 1);

	m_versionText.setText(APP_NAME + std::string(" v0.0.1"));
	m_versionText.setColor(Color::white);

	m_positionText.setPosition(0, 10, 0);
	m_positionText.setColor(Color::white);
}

void DebugOverlay::update() {
	std::stringstream stream;
	stream << "x: " << floorf(m_camera.x()) << " | ";
	stream << "y: " << floorf(m_camera.y()) << " | ";
	stream << "z: " << floorf(m_camera.z());

	m_positionText.setText(stream.str());
}

void DebugOverlay::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_versionText, states);
	target.draw(m_positionText, states);
}

