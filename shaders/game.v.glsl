#version 120

attribute vec2 coord2d;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

void main() {
	mat4 MVP = u_projectionMatrix * u_viewMatrix;
	
	gl_Position = MVP * vec4(coord2d, 0.0, 1.0);
}

