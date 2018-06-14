/*
 * =====================================================================================
 *
 *       Filename:  Chunk.cpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 17:31:32
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Chunk.hpp"

Chunk::Chunk(s32 x, s32 y, s32 z, Texture &texture) : m_texture(texture) {
	m_x = x;
	m_y = y;
	m_z = z;

	m_isChanged = false;
	m_isInitialized = false;
	m_isGenerated = false;

	m_surroundingChunks[0] = nullptr;
	m_surroundingChunks[1] = nullptr;
	m_surroundingChunks[2] = nullptr;
	m_surroundingChunks[3] = nullptr;
}

// FIXME: Find a better way to do that
void Chunk::update() {
	if (!m_isChanged || m_data.empty()) return;

	m_isChanged = false;

	static const float cubeCoords[6 * 4 * 3] = {
		0, 1, 1,
		0, 1, 0,
		0, 0, 0,
		0, 0, 1,

		1, 0, 1,
		1, 0, 0,
		1, 1, 0,
		1, 1, 1,

		0, 0, 0,
		1, 0, 0,
		1, 0, 1,
		0, 0, 1,

		0, 1, 1,
		1, 1, 1,
		1, 1, 0,
		0, 1, 0,

		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1,

		0, 1, 0,
		1, 1, 0,
		1, 0, 0,
		0, 0, 0,
	};

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;

	vertices.reserve(width * height * depth * 6 * 4 * 3);
	normals.reserve(width * height * depth * 6 * 4 * 3);
	texCoords.reserve(width * height * depth * 6 * 4 * 2);

	// Needed in the loop (avoid a lot of glm::vec3 creation)
	glm::vec3 a, b, c, v1, v2, normal;

	for(u8 z = 0 ; z < depth ; z++) {
		for(u8 y = 0 ; y < height ; y++) {
			for(u8 x = 0 ; x < width ; x++) {
				Block *block = getBlock(x, y, z);

				if(!block->id()) {
					continue;
				}

				const glm::vec4 &blockTexCoords = block->getTexCoords();

				float cubeTexCoords[2 * 4 * 6] = {
					blockTexCoords.x, blockTexCoords.w,
					blockTexCoords.z, blockTexCoords.w,
					blockTexCoords.z, blockTexCoords.y,
					blockTexCoords.x, blockTexCoords.y
				//	0.0f, 1.0f,
				//	1.0f, 1.0f,
				//	1.0f, 0.0f,
				//	0.0f, 0.0f
				};

				for(int i = 1 ; i < 6 ; i++) {
					memcpy(&cubeTexCoords[2 * 4 * i], &cubeTexCoords[0], 2 * 4 * sizeof(float));
				}

				for(u8 i = 0 ; i < 6 ; i++) {
					// Skip hidden faces
					Block *surroundingBlocks[4] = {nullptr, nullptr, nullptr, nullptr};

					if (m_surroundingChunks[0]) surroundingBlocks[0] = m_surroundingChunks[0]->getBlock(width - 1, y, z);
					if (m_surroundingChunks[1]) surroundingBlocks[1] = m_surroundingChunks[1]->getBlock(0, y, z);
					if (m_surroundingChunks[2]) surroundingBlocks[2] = m_surroundingChunks[2]->getBlock(x, y, depth - 1);
					if (m_surroundingChunks[3]) surroundingBlocks[3] = m_surroundingChunks[3]->getBlock(x, y, 0);

					if((x > 0           && getBlock(x - 1, y, z)->id() && i == 0)
					|| (x < width - 1   && getBlock(x + 1, y, z)->id() && i == 1)
					|| (y > 0           && getBlock(x, y - 1, z)->id() && i == 2)
					|| (y < height - 1  && getBlock(x, y + 1, z)->id() && i == 3)
					|| (z > 0           && getBlock(x, y, z - 1)->id() && i == 5)
					|| (z < depth - 1   && getBlock(x, y, z + 1)->id() && i == 4)
					|| (x == 0          && surroundingBlocks[0] && surroundingBlocks[0]->id()  && i == 0)
					|| (x == width - 1  && surroundingBlocks[1] && surroundingBlocks[1]->id()          && i == 1)
					|| (z == 0          && surroundingBlocks[2] && surroundingBlocks[2]->id()  && i == 5)
					|| (z == depth - 1  && surroundingBlocks[3] && surroundingBlocks[3]->id()          && i == 4)
					) {
						continue;
					}

					// Three points of the face
					a.x = cubeCoords[i * 12 + 0];
					a.y = cubeCoords[i * 12 + 1];
					a.z = cubeCoords[i * 12 + 2];

					b.x = cubeCoords[i * 12 + 3];
					b.y = cubeCoords[i * 12 + 4];
					b.z = cubeCoords[i * 12 + 5];

					c.x = cubeCoords[i * 12 + 6];
					c.y = cubeCoords[i * 12 + 7];
					c.z = cubeCoords[i * 12 + 8];

					// Computing two vectors
					v1 = b - a;
					v2 = c - a;

					// Computing face normal (already normalized because cubeCoords are normalized)
					normal = glm::cross(v1, v2);

					// Store vertex information
					for(u8 j = 0 ; j < 4 ; j++) {
						vertices.push_back(x + cubeCoords[i * 12 + j * 3]);
						vertices.push_back(y + cubeCoords[i * 12 + j * 3 + 1]);
						vertices.push_back(z + cubeCoords[i * 12 + j * 3 + 2]);

						normals.push_back(normal.x);
						normals.push_back(normal.y);
						normals.push_back(normal.z);

						texCoords.push_back(cubeTexCoords[i * 8 + j * 2]);
						texCoords.push_back(cubeTexCoords[i * 8 + j * 2 + 1]);
					}
				}
			}
		}
	}

	vertices.shrink_to_fit();
	normals.shrink_to_fit();
	texCoords.shrink_to_fit();

	VertexBuffer::bind(&m_vbo);

	m_vbo.setData((vertices.size() + normals.size() + texCoords.size()) * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	m_vbo.updateData(0, vertices.size() * sizeof(float), vertices.data());
	m_vbo.updateData(vertices.size() * sizeof(float), normals.size() * sizeof(float), normals.data());
	m_vbo.updateData((vertices.size() + normals.size()) * sizeof(float), texCoords.size() * sizeof(float), texCoords.data());

	VertexBuffer::bind(nullptr);

	m_verticesCount = vertices.size();
	m_normalsCount = normals.size();
}

void Chunk::addBlock(const glm::vec3 &pos, u32 id) {
	m_data.push_back(std::unique_ptr<Block>(new Block(pos, id)));
}

Block *Chunk::getBlock(s8 x, s8 y, s8 z) const {
	u16 i = y + x * height + z * height * width;
	if(i < m_data.size()) {
		return m_data[i].get();
	} else {
		return nullptr;
	}
}

void Chunk::setBlock(s8 x, s8 y, s8 z, u32 id) {
	glm::vec3 pos;
	pos.x = x + m_x * width;
	pos.y = y + m_y * height;
	pos.z = z + m_z * depth;

	m_data.at(y + x * height + z * height * width).reset(new Block(pos, id));

	m_isChanged = true;

	if(x == 0         && left())  { left()->m_isChanged = true; }
	if(x == width - 1 && right()) { right()->m_isChanged = true; }
	if(z == 0         && front()) { front()->m_isChanged = true; }
	if(z == depth - 1 && back())  { back()->m_isChanged = true; }
}

// FIXME: Implement vertex attributes in VertexBuffer and remove most of the code here
void Chunk::draw(RenderTarget &target, RenderStates states) const {
	if(m_verticesCount == 0) return;

	VertexBuffer::bind(&m_vbo);

	states.shader->enableVertexAttribArray("coord3d");
	states.shader->enableVertexAttribArray("normal");
	states.shader->enableVertexAttribArray("texCoord");

	glVertexAttribPointer(states.shader->attrib("coord3d"),  3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(states.shader->attrib("normal"),   3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(m_verticesCount * sizeof(float)));
	glVertexAttribPointer(states.shader->attrib("texCoord"), 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((m_verticesCount + m_normalsCount) * sizeof(float)));

	states.texture = &m_texture;

	target.draw(m_vbo, 0, m_verticesCount / 3, states);

	states.shader->disableVertexAttribArray("texCoord");
	states.shader->disableVertexAttribArray("normal");

	// drawOutlines(target, states);
}

// FIXME: Use the renderer to do that
void Chunk::drawOutlines(RenderTarget &, RenderStates states) const {
	Shader::bind(states.shader);
	VertexBuffer::bind(&m_vbo);

	states.shader->enableVertexAttribArray("coord3d");

	for(u32 i = 0 ; i < m_verticesCount / 3 ; i += 4) {
		glDrawArrays(GL_LINE_LOOP, i, 4);
	}

	states.shader->disableVertexAttribArray("coord3d");

	VertexBuffer::bind(nullptr);
	Shader::bind(nullptr);
}

