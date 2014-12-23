#version 120

attribute vec3 coord3d;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec4 v_coord3d;
varying vec4 v_normal;
varying vec2 v_texCoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewProjectionMatrix;

varying float v_depth;

void main() {
	v_coord3d = u_modelMatrix * vec4(coord3d, 1.0);
	v_depth = length((u_viewProjectionMatrix * v_coord3d).xyz);
	
	gl_Position = u_viewProjectionMatrix * v_coord3d;
	
	v_normal = vec4(normal, 1.0);
	v_texCoord = texCoord;
}

