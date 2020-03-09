#version 120

uniform vec4 u_fogColor;

vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);

	return mix(u_fogColor, color, fog);
}

