#version 120

attribute vec2 coord2d;
attribute vec2 texCoord;

varying vec2 v_texCoord;
varying vec2 v_coord2d;

void main() {
	v_texCoord = texCoord;
	v_coord2d = coord2d;

	gl_Position = vec4(coord2d.x, coord2d.y, 0.0, 1.0);
}

