/*
 * =====================================================================================
 *
 *       Filename:  RenderTarget.cpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 19:55:28
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "IDrawable.hpp"
#include "RenderTarget.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"

const RenderStates RenderStates::Default{};

void RenderTarget::draw(const IDrawable &drawable, const RenderStates &states) {
	drawable.draw(*this, states);
}

void RenderTarget::draw(const VertexBuffer &vertexBuffer, GLenum mode, std::size_t firstVertex, std::size_t vertexCount, const RenderStates &states) {
	if (!states.shader) return;

	Shader::bind(states.shader);

	VertexBuffer::bind(&vertexBuffer);

	states.shader->setUniform("u_projectionMatrix", (states.projectionMatrix) ? *states.projectionMatrix : glm::mat4{1});
	states.shader->setUniform("u_modelMatrix", (states.modelMatrix) ? *states.modelMatrix : glm::mat4{1});
	states.shader->setUniform("u_viewMatrix", (states.viewMatrix) ? *states.viewMatrix : glm::mat4{1});

	states.shader->enableVertexAttribArray("coord3d");
	states.shader->enableVertexAttribArray("normal");
	states.shader->enableVertexAttribArray((states.texture) ? "texCoord" : "color");
	states.shader->enableVertexAttribArray("lightValue");

	// glVertexAttribPointer(states.shader->attrib("coord3d"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(states.shader->attrib("coord3d"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, coord3d)));
	glVertexAttribPointer(states.shader->attrib("normal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));
	if (states.texture)
		glVertexAttribPointer(states.shader->attrib("texCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, texCoord)));
	else
		glVertexAttribPointer(states.shader->attrib("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, color)));
	glVertexAttribPointer(states.shader->attrib("lightValue"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, lightValue)));


	if (states.texture)
		Texture::bind(states.texture);

	glDrawArrays(mode, firstVertex, vertexCount);

	Texture::bind(nullptr);

	states.shader->disableVertexAttribArray("lightValue");
	states.shader->disableVertexAttribArray((states.texture) ? "texCoord" : "color");
	states.shader->disableVertexAttribArray("normal");
	states.shader->disableVertexAttribArray("coord3d");

	VertexBuffer::bind(nullptr);

	Shader::bind(nullptr);
}

