#version 120

attribute vec4 color;
attribute vec4 coord3d;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec2 lightValue;
attribute float blockType;
attribute float ambientOcclusion;

varying vec4 v_color;
varying vec4 v_coord3d;
varying vec4 v_normal;
varying vec2 v_texCoord;
varying vec2 v_lightValue;
varying float v_ambientOcclusion;

varying float v_blockFace;
varying float v_blockID;
varying float v_dist;

uniform mat4 u_modelMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

uniform int u_renderDistance;
uniform int u_time;

void main() {
	vec3 finalPos = coord3d.xyz;
	if (blockType == 8) { // Water
		// finalPos.y += sin((u_time / 1000.0 + mod(finalPos.x, 16)) * 1.75) / 10.0;
		// finalPos.y += cos((u_time / 1000.0 + mod(finalPos.z, 16)) * 1.75) / 10.0;
		// finalPos.y -= 0.25;
		// finalPos.y -= 1.0 / 16.0;
		finalPos.y += sin(u_time / 1000.0) / 16.0 - 0.125;
	}
	else if (blockType == 4) { // Leaves
		finalPos.xz += sin((u_time / 1000.0 + finalPos.x) * 2) / 30.0;
		finalPos.xz += cos((u_time / 1000.0 + finalPos.z) * 2) / 30.0;
	}

	// Used for lighting
	v_coord3d = u_modelMatrix * vec4(finalPos, 1.0);
	v_normal = vec4(normal, 1.0);

	v_color = color;
	v_texCoord = texCoord;
	v_lightValue = lightValue;

	if (ambientOcclusion != 5) {
		const float aovalues[] = float[](0.5, 0.75, 0.9, 1.0);
		v_ambientOcclusion = aovalues[int(ambientOcclusion)];
	} else {
		v_ambientOcclusion = 1.0;
	}

	v_blockFace = coord3d.w;
	v_blockID = blockType;

	// Distance from eye
	v_dist = length(u_viewMatrix * v_coord3d);

	gl_Position = u_projectionMatrix * u_viewMatrix * v_coord3d;
}

