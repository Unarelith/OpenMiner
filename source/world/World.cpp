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
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "Config.hpp"
#include "World.hpp"

Block *World::selectedBlock = nullptr;
Chunk *World::selectedChunk = nullptr;

World::World() {
	m_width = 100;
	m_depth = 100;

	//m_texture.load("textures/cobblestone.bmp");
	m_texture.load("textures/blocks.png");

	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
			m_chunks.push_back(std::unique_ptr<Chunk>(new Chunk(x, 0, z, m_texture)));
		}
	}

	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
			if(x > -m_width / 2)	 getChunk(x, z)->setLeft(getChunk(x - 1, z));
			if(x <  m_width / 2 - 1) getChunk(x, z)->setRight(getChunk(x + 1, z));
			if(z > -m_depth / 2)	 getChunk(x, z)->setFront(getChunk(x, z - 1));
			if(z <  m_depth / 2 - 1) getChunk(x, z)->setBack(getChunk(x, z + 1));
		}
	}
}

void World::draw(Shader &shader, const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix) {
	float ud = 1000.0;
	s32 ux = 0;
	s32 uz = 0;

	shader.setUniform("u_renderDistance", renderDistance * Chunk::width);

	for(auto &it : m_chunks) {
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),
		                                       glm::vec3(it->x() * Chunk::width,
		                                                 it->y() * Chunk::height,
		                                                 it->z() * Chunk::depth));

		// Is the chunk closer enough?
		glm::vec4 center = viewMatrix * modelMatrix * glm::vec4(Chunk::width  / 2,
		                                                        Chunk::height / 2,
		                                                        Chunk::depth  / 2, 1);

		if(glm::length(center) > (renderDistance + 1) * Chunk::width) {
			continue;
		}

		// Is this chunk on the screen?
		center = projectionMatrix * center;

		float d = glm::length(center);
		center.x /= center.w;
		center.y /= center.w;

		// If it is behind the camera, don't bother drawing it
		if(center.z < -Chunk::height / 2) {
			continue;
		}

		// It it is outside the screen, don't bother drawing it
		if(fabsf(center.x) > 1 + fabsf(Chunk::height * 2 / center.w)
		|| fabsf(center.y) > 1 + fabsf(Chunk::height * 2 / center.w)) {
			continue;
		}

		if(!it->isInitialized()) {
			if(d < ud) {
				ud = d;
				ux = it->x();
				uz = it->z();
			}

			continue;
		}

		shader.setUniform("u_modelMatrix", modelMatrix);

		it->draw(shader);
	}

	if(ud < 1000) {
		m_terrainGenerator.generate(*getChunk(ux, uz));

		getChunk(ux, uz)->setInitialized(true);

		if(getChunk(ux, uz)->left())  m_terrainGenerator.generate(*getChunk(ux, uz)->left());
		if(getChunk(ux, uz)->right()) m_terrainGenerator.generate(*getChunk(ux, uz)->right());
		if(getChunk(ux, uz)->front()) m_terrainGenerator.generate(*getChunk(ux, uz)->front());
		if(getChunk(ux, uz)->back())  m_terrainGenerator.generate(*getChunk(ux, uz)->back());
	}
}

Chunk *World::getChunk(s32 x, s32 z) {
	x += m_width / 2;
	z += m_depth / 2;

	return m_chunks.at(x + z * m_width).get();
}

// FIXME: Move to a math module
bool World::intersectionLinePlane(const glm::vec3 &normal, const glm::vec3 &planePoint, const glm::vec3 &lineOrigPoint, const glm::vec3 &directionVector, float *distance) {
	float p1 = directionVector.x * normal.x + directionVector.y * normal.y + directionVector.z * normal.z; // First point to be tested

	if(p1 == 0) return false; // Degenerate case

	glm::vec3 u = glm::vec3(planePoint.x - lineOrigPoint.x,
					  planePoint.y - lineOrigPoint.y,
					  planePoint.z - lineOrigPoint.z);

	float p2 = u.x * normal.x + u.y * normal.y + u.z * normal.z; // Second point to be tested

	float k = p2 / p1;

	if((k < 0) || (k > 5)) return false;

	// Intersection point
	glm::vec3 i = glm::vec3(lineOrigPoint.x + k * directionVector.x,
					  lineOrigPoint.y + k * directionVector.y,
					  lineOrigPoint.z + k * directionVector.z);

	glm::vec3 v = glm::vec3(i.x - planePoint.x,
					  i.y - planePoint.y,
					  i.z - planePoint.z);

	float size = 0.5;

	if(v.x >= -size && v.x <= size && v.y >= -size && v.y <= size && v.z >= -size && v.z <= size) {
		if(distance != nullptr) *distance = k;
		return true;
	} else {
		return false;
	}
}

// Front right = 0 | Front left = 1
// Back right = 2 | Back left = 3
// Top = 4 | Bottom = 5
// FIXME: Move to a math module
bool World::intersectionLineCube(int cubeX, int cubeY, int cubeZ, const glm::vec3 &lineOrigPoint, const glm::vec3 &directionVector, float *distance, s8 *face) {
	glm::vec3 planePoint[6] = {
		glm::vec3(cubeX + 0.5, cubeY + 1, cubeZ + 0.5), // back
		glm::vec3(cubeX + 1, cubeY + 0.5, cubeZ + 0.5), // right
		glm::vec3(cubeX, cubeY + 0.5, cubeZ + 0.5), // left
		glm::vec3(cubeX + 0.5, cubeY, cubeZ + 0.5), // front
		glm::vec3(cubeX + 0.5, cubeY + 0.5, cubeZ + 1), // top
		glm::vec3(cubeX + 0.5, cubeY + 0.5, cubeZ) // bottom
	};

	glm::vec3 normal[6] = {
		glm::vec3(0, 1, 0), // back
		glm::vec3(1, 0, 0), // right
		glm::vec3(-1, 0, 0), // left
		glm::vec3(0, -1, 0), // front
		glm::vec3(0, 0, 1), // top
		glm::vec3(0, 0, -1) // bottom
	};

	float shortestDistance = DIST_FAR;
	float dist = DIST_FAR + 1.0;
	int nearestFace = -1;

	for (int i = 0; i < 6; i++) {
		bool result = intersectionLinePlane(normal[i], planePoint[i], lineOrigPoint, directionVector, &dist);
		if (result && (dist < shortestDistance)) {
			shortestDistance = dist;
			nearestFace = i;
		}
	}

	if (nearestFace < 0) {
		return false;
	} else {
		if (distance != nullptr) *distance = shortestDistance;
		if (face != nullptr) *face = nearestFace;
		return true;
	}
}

#include <unordered_map>
#include <unistd.h>

// FIXME: MOVE THIS FUNCTION
void World::testCubes(Camera &camera) {
	glm::vec3 linePoint = glm::vec3(camera.x(),
	                                camera.y(),
	                                camera.z());

	glm::vec3 directionVector = glm::vec3(camera.pointTargetedX() - camera.x(),
	                                      camera.pointTargetedY() - camera.y(),
	                                      camera.pointTargetedZ() - camera.z());

	Chunk *currentChunk = getChunk(camera.x() / Chunk::width, camera.z() / Chunk::depth);
	float distance = DIST_FAR;
	Block *block = nullptr;
	int face = -1;
	Chunk *chunk = nullptr;
	const std::vector<std::unique_ptr<Block>> *blocks = nullptr;
	for(unsigned short i = 0 ; i < 9 ; i++) {
		if(i == 8) blocks = &currentChunk->data();
		else if(i < 8) {
			if(currentChunk->getSurroundingChunk(i) == nullptr) continue;
			blocks = &currentChunk->getSurroundingChunk(i)->data();
		}

		for(auto &it : *blocks) {
			if(it->pos().z < linePoint.z - 10 || it->pos().z > linePoint.z + 10) continue;
			if(it->pos().y < linePoint.y - 10 || it->pos().y > linePoint.y + 10) continue;
			if(it->pos().x < linePoint.x - 10 || it->pos().x > linePoint.x + 10) continue;

			// it->setSelected(false, -1);

			float d = -1;
			s8 f = -1;

			bool result = intersectionLineCube(it->pos().x, it->pos().y, it->pos().z, linePoint, directionVector, &d, &f);

			if(result && (d < distance) && (d < 5)) {
				distance = d;
				block = it.get();
				face = f;
				if(i == 8) chunk = currentChunk;
				else if(i < 8) chunk = currentChunk->getSurroundingChunk(i);
				write(1, result ? "y" : "n", 1);
			}
		}
	}

	if(block != nullptr) {
		selectedBlock = block;
		// block->setSelected(true, face);
	} else {
		// selectedCube->setSelected(false, -1);
	}

	if(chunk != nullptr) {
		selectedChunk = chunk;
	} else {
		selectedChunk = nullptr;
	}
}

