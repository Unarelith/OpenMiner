#version 120

attribute vec3 color;
attribute vec3 coord3d;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec4 v_color;
varying vec4 v_coord3d;
varying vec4 v_normal;
varying vec2 v_texCoord;

varying vec4 v_viewSpace;

uniform mat4 u_modelMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

void main() {
	v_coord3d = u_modelMatrix * vec4(coord3d, 1.0);
	v_viewSpace = u_viewMatrix * v_coord3d;
	
	gl_Position = u_projectionMatrix * v_viewSpace;
	
	v_color = vec4(color, 1.0);
	v_normal = vec4(normal, 1.0);
	v_texCoord = texCoord;
}

