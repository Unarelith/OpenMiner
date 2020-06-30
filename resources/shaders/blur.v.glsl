#version 120

attribute vec2 coord2d;
attribute vec2 texCoord;

varying vec2 v_texCoord;

void main() {
	v_texCoord = texCoord;

	gl_Position = vec4(coord2d, 0, 1);
}

