#version 120

varying vec2 v_lightValue;

uniform int u_time;

vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);

	const float pi = 3.1415927;
	const float frequency = 256000;

	float time = mod(u_time, 512000);
	float sunlight = clamp((1 + cos(2 * pi / frequency * time) * 4.0), 0.0, 1.0);
	float red = clamp(sunlight - 0.55 - 0.2, 0.0, 0.45);
	float green = clamp(sunlight - 0.35 - 0.2, 0.0, 0.65);
	float blue = clamp(sunlight - 0.09, 0.0, 0.91);
	return mix(vec4(red, green, blue, 1.0), color, fog);
}

