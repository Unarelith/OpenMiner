#version 120

attribute vec4 color;
attribute vec4 coord3d;
attribute vec2 texCoord;

varying vec4 v_color;
varying vec4 v_coord3d;
varying vec2 v_texCoord;
varying float v_faceValue;

uniform mat4 u_modelMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

void main() {
	v_faceValue = coord3d.w;
	v_coord3d = u_modelMatrix * vec4(coord3d.xyz, 1.0);
	v_color = color;
	v_texCoord = texCoord;

	gl_Position = u_projectionMatrix * u_viewMatrix * v_coord3d;
}

