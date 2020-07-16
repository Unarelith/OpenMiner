#version 120

uniform vec4 u_fogColor;
uniform vec4 u_skyColor;

vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);

	return mix(vec4(u_skyColor.r, u_skyColor.g, u_skyColor.b, u_fogColor.a), color, fog);
}

