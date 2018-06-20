#version 120

attribute vec4 color;
attribute vec4 coord3d;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec4 v_color;
varying vec4 v_coord3d;
varying vec4 v_normal;
varying vec2 v_texCoord;

varying float v_blockID;
varying float v_dist;

uniform mat4 u_modelMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

uniform int u_renderDistance;
uniform int u_time;

void main() {
	vec3 finalPos = coord3d.xyz;
	if (coord3d.w == 8) {
		finalPos.y += sin((u_time / 1000.0 + mod(finalPos.x, 16)) * 1.75) / 10.0;
		finalPos.y += cos((u_time / 1000.0 + mod(finalPos.z, 16)) * 1.75) / 10.0;
		finalPos.y -= 0.25;
		finalPos.y -= 1 / 16;
		/* finalPos.y += sin(u_time / 1000.0) / 8.0 - 0.25; */
	}

	// Used for lighting
	v_coord3d = u_modelMatrix * vec4(finalPos, 1.0);
	v_normal = vec4(normal, 1.0);

	v_blockID = coord3d.w;

	v_color = color;
	v_texCoord = texCoord;

	// Distance from eye
	v_dist = length(u_viewMatrix * v_coord3d);

	gl_Position = u_projectionMatrix * u_viewMatrix * v_coord3d;
}

