#version 120

varying vec2 v_lightValue;

uniform int u_time;

vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);

	const float pi = 3.1415927;
	const float frequency = 256000;

	float time = mod(u_time, 512000);
	float sunlight = clamp(1 + cos(2 * pi / frequency * time), 0.25, 1);

	return mix(vec4(0.53*sunlight, 0.81*sunlight, 0.92*sunlight, 1.0), color, fog);
}

