#version 120

uniform vec4 u_fogColor;

uniform float u_time;

vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);

	const float pi = 3.1415927;

	float sunlight = clamp((1 + sin(2 * pi * u_time) * 4.0), 0.0, 1.0);

	float red = clamp(sunlight - (1 - u_fogColor.r), 0.0, u_fogColor.r);
	float green = clamp(sunlight - (1 - u_fogColor.g), 0.0, u_fogColor.g);
	float blue = clamp(sunlight - (1 - u_fogColor.b), 0.0, u_fogColor.b);

	return mix(vec4(red, green, blue, u_fogColor.a), color, fog);
}

