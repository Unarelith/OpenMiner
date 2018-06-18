#version 120

attribute vec4 color;
attribute vec4 coord3d;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec4 v_color;
varying vec4 v_coord3d;
varying vec4 v_normal;
varying vec2 v_texCoord;

varying float v_dist;

uniform mat4 u_modelMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

uniform int u_renderDistance;

void main() {
	// Used for lighting
	v_coord3d = u_modelMatrix * coord3d;
	v_normal = vec4(normal, 1.0);

	v_color = color;
	v_texCoord = texCoord;

	// Distance from eye
	v_dist = length(u_viewMatrix * v_coord3d);

	gl_Position = u_projectionMatrix * u_viewMatrix * v_coord3d;
}

