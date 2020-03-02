#version 120

varying vec4 v_coord3d;
varying vec4 v_color;
varying vec2 v_lightValue;
varying float v_ambientOcclusion;

varying float v_blockFace;
varying float v_dist;

uniform int u_renderDistance;
uniform int u_time;
// Get current pixel color
vec4 getColor();

// Get light color
vec4 light(vec4 color, vec3 lightColor, vec4 lightPosition, float ambientIntensity, float diffuseIntensity);

// Apply fog
vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd);

void main() {
	// Needed to prevent bad interpolation on some systems
	// Refer to #23 for more informations
	float blockFace = floor(v_blockFace + 0.5);
	float lightCheck = floor(v_lightValue.x + 0.5);

	// Discard if the pixel is too far away
	if(blockFace != -1. && v_dist > u_renderDistance) discard;

	// Get current pixel color and apply multiplier on grayscale textures
	vec4 color = getColor();
	if (blockFace != -1 && color != v_color && color.r == color.g && color.g == color.b) {
		color *= v_color;
	}

	// Very cheap "transparency": don't draw pixels with a low alpha value
	if(color.a < 0.3 && blockFace != -1.) discard;

	// FIXME: FINISH THIS WITH PROPER CODE AND SUN BASIC DISPLAY
	// int maxTime = 5 * 1000;
	// float time = mod(u_lightPosition, maxTime) / maxTime * 2 - 1;
	// vec3 lightPosition = vec3(0.0, sin(time) * 40, cos(time) * 40);
	// color *= light(vec3(1.0, 1.0, 1.0), vec4(lightPosition, 1.0), 0.5, 0.5);

	//variables for time and other math stuff

	float minBrightness = 2.0 / 16.0;

	if (lightCheck != -1.) {
		float pi = 3.14159265358979323846264338327950288419716939937510; // useless? high precision pi stuff
		//		the block is pretty much a super primitive version of the 3 lines after it
		//		mostly due to deletboi not having any idea what clamp is and
		//		having never worked in a professional environment
		// float sunlight = v_lightValue.x;
		// if (ticktime >=0 && ticktime <= 256000)
		// {
		// 	sunlight *= min(sin(ticktime/256000 + pi*u_time/256000), 1);
		// 	// color = light(color, vec3(1.0, 1.0, 1.0), v_coord3d, ambientIntensity, diffuseIntensity);
		// }
		// else if(ticktime > 256000 && ticktime <512000)
		// {
		// 	sunlight *= max(cos(ticktime/256000 + pi*u_time/256000), 0);
		// }

		const float frequency = 256000;
		float time = mod(u_time, 512000);
		float sunlight = clamp(v_lightValue.x * 0.5 * (1 + sin(2 * pi * time / frequency)), 0, 15);

		float ambientIntensity = max(max(sunlight, v_lightValue.y) / 16.0, minBrightness);
		float diffuseIntensity = max(sunlight, v_lightValue.y) / 32.0;

		// These numbers should be in sync with enum BlockFace in TilesDef.hpp
		// Bottom
		if (blockFace == 4.)
			ambientIntensity = max(ambientIntensity * 0.6, minBrightness);
		// West or East
		if (blockFace == 0. || blockFace == 1.)
			ambientIntensity = max(ambientIntensity * 0.75, minBrightness);
		// South or North
		if (blockFace == 2. || blockFace == 3.)
			ambientIntensity = max(ambientIntensity * 0.9, minBrightness);

			color = light(color, vec3(1.0, 1.0, 1.0), v_coord3d, ambientIntensity, diffuseIntensity);
		// color = vec4(0, 0, v_lightValue.x / 16.0, 1);
	}

	color.rgb *= v_ambientOcclusion;

	color = fog(color, v_dist, u_renderDistance - 32, u_renderDistance);

	gl_FragColor = color;
}
