#version 120

varying float v_blockFace;
varying float v_blockID;
varying float v_dist;

varying vec4 v_coord3d;
varying vec2 v_lightValue;

uniform int u_renderDistance;

// Get current pixel color
vec4 getColor();

// Get light color
vec4 light(vec4 color, vec3 lightColor, vec4 lightPosition, float ambientIntensity, float diffuseIntensity);

// Apply fog
vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd);

void main() {
	// Discard if the pixel is too far away
	if(v_blockID != -1 && v_dist > u_renderDistance) discard;

	vec4 color = getColor();
	if (v_blockID == 8) {
		color.w = 0.85;
	}
	else if (v_blockID == 4) {
		color += vec4(-0.5, -0.15, -0.4, 0);
	}
	else if (v_blockID == 3) {
		if (color.x == color.y && color.x == color.z)
			color += vec4(-0.3, -0.1, -0.25, 0);
	}


	// FIXME: FINISH THIS WITH PROPER CODE AND SUN BASIC DISPLAY
	// int maxTime = 5 * 1000;
	// float time = mod(u_lightPosition, maxTime) / maxTime * 2 - 1;
	// vec3 lightPosition = vec3(0.0, sin(time) * 40, cos(time) * 40);
	// color *= light(vec3(1.0, 1.0, 1.0), vec4(lightPosition, 1.0), 0.5, 0.5);

	if (v_lightValue.x != -1) {
		float ambientIntensity = max(max(v_lightValue.x, v_lightValue.y) / 16.0, 2.0 / 16.0);
		float diffuseIntensity = max(v_lightValue.x, v_lightValue.y) / 32.0;

		if (v_blockFace == 2)
			ambientIntensity = max(ambientIntensity * 0.7, 2.0 / 16.0);
		if (v_blockFace == 0 || v_blockFace == 1 || v_blockFace == 4 || v_blockFace == 5)
			ambientIntensity = max(ambientIntensity * 0.8, 2.0 / 16.0);

		color = light(color, vec3(1.0, 1.0, 1.0), v_coord3d, ambientIntensity, diffuseIntensity);
	}

	color = fog(color, v_dist, u_renderDistance - 32, u_renderDistance);

	gl_FragColor = color;
}

