#version 120

vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);

	return mix(vec4(0.196078, 0.6, 0.8, 1.0), color, fog);
}

