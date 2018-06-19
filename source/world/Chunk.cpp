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
}

void Chunk::update() {
	if (!m_isChanged || m_data.empty()) return;

	m_isChanged = false;

	m_verticesCount = m_builder.buildChunk(*this, m_vbo);
}

Block *Chunk::getBlock(int x, int y, int z) const {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->getBlock(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getBlock(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getBlock(x, y + Chunk::height, z) : 0;
	if(y >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getBlock(x, y - Chunk::height, z) : 0;
	if(z < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getBlock(x, y, z + Chunk::depth) : 0;
	if(z >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getBlock(x, y, z - Chunk::depth) : 0;
	return m_data[x][y][z].get();
}

void Chunk::setBlock(int x, int y, int z, u32 type) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setBlock(x + Chunk::width, y, z, type); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setBlock(x - Chunk::width, y, z, type); return; }
	if(y < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setBlock(x, y + Chunk::height, z, type); return; }
	if(y >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setBlock(x, y - Chunk::height, z, type); return; }
	if(z < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setBlock(x, y, z + Chunk::depth, type); return; }
	if(z >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setBlock(x, y, z - Chunk::depth, type); return; }

	m_data[x][y][z].reset(new Block(type));

	m_isChanged = true;

	if(x == 0          && left())  { left()->m_isChanged = true; }
	if(x == width - 1  && right()) { right()->m_isChanged = true; }
	if(y == 0          && below()) { below()->m_isChanged = true; }
	if(y == height - 1 && above()) { above()->m_isChanged = true; }
	if(z == 0          && front()) { front()->m_isChanged = true; }
	if(z == depth - 1  && back())  { back()->m_isChanged = true; }
}

void Chunk::draw(RenderTarget &target, RenderStates states) const {
	if(m_verticesCount == 0) return;

	states.texture = &m_texture;

	target.draw(m_vbo, GL_QUADS, 0, m_verticesCount, states);

	// drawOutlines(target, states);
}

// FIXME: Use the renderer to do that
void Chunk::drawOutlines(RenderTarget &, RenderStates states) const {
	Shader::bind(states.shader);
	VertexBuffer::bind(&m_vbo);

	states.shader->enableVertexAttribArray("coord3d");

	for(u32 i = 0 ; i < m_verticesCount ; i += 4) {
		glDrawArrays(GL_LINE_LOOP, i, 4);
	}

	states.shader->disableVertexAttribArray("coord3d");

	VertexBuffer::bind(nullptr);
	Shader::bind(nullptr);
}

