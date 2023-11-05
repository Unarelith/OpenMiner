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
#ifndef ANIMATIONCOMPONENT_HPP_
#define ANIMATIONCOMPONENT_HPP_

#include <vector>

#include "IntTypes.hpp"
#include "ISerializable.hpp"
#include "Network.hpp"
#include "NetworkUtils.hpp"

enum class AnimationType {
	Rotation,
	Translation,
};

struct AnimationData : public ISerializable {
	AnimationType type;

	union {
		struct {
			float axisX;
			float axisY;
			float axisZ;

			float angle;
		} rotation;

		struct {
			float dx, dy, dz;
			float cx, cy, cz;
			float min, max;
			bool loop;
		} translation;
	};

	void serialize(sf::Packet &packet) const override {
		packet << u8(type);
		if (type == AnimationType::Rotation) {
			packet << rotation.axisX << rotation.axisY << rotation.axisZ << rotation.angle;
		}
		else if (type == AnimationType::Translation) {
			packet << translation.dx << translation.dy << translation.dz
				<< translation.cx << translation.cy << translation.cz
				<< translation.min << translation.max << translation.loop;
		}
	}

	void deserialize(sf::Packet &packet) override {
		u8 type;
		packet >> type;
		if (type == u8(AnimationType::Rotation)) {
			packet >> rotation.axisX >> rotation.axisY >> rotation.axisZ >> rotation.angle;
		}
		else if (type == u8(AnimationType::Translation)) {
			packet >> translation.dx >> translation.dy >> translation.dz
				>> translation.cx >> translation.cy >> translation.cz
				>> translation.min >> translation.max >> translation.loop;
		}

		this->type = AnimationType(type);
	}
};

struct AnimationComponent : public ISerializable {
	void addRotation(float axisX, float axisY, float axisZ, float angle) {
		list.emplace_back();
		AnimationData &data = list.back();

		data.type = AnimationType::Rotation;

		data.rotation.axisX = axisX;
		data.rotation.axisY = axisY;
		data.rotation.axisZ = axisZ;

		data.rotation.angle = angle;
	}

	void addTranslation(float dx, float dy, float dz, float min, float max, bool loop) {
		list.emplace_back();
		AnimationData &data = list.back();

		data.type = AnimationType::Translation;

		data.translation.dx = dx;
		data.translation.dy = dy;
		data.translation.dz = dz;

		data.translation.cx = 0;
		data.translation.cy = 0;
		data.translation.cz = 0;

		data.translation.min = min;
		data.translation.max = max;

		data.translation.loop = loop;
	}

	void serialize(sf::Packet &packet) const override { packet << list; }
	void deserialize(sf::Packet &packet) override { packet >> list; }
	Network::Command packetType = Network::Command::EntityAnimation;

	std::vector<AnimationData> list;

	bool isUpdated = true;
};

#endif // ANIMATIONCOMPONENT_HPP_
