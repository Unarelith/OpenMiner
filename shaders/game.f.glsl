#version 120

varying float v_dist;

uniform int u_renderDistance;

// Get current pixel color
vec4 getColor();

// Get light color
vec4 light(vec3 lightColor, vec4 lightPosition, float ambientIntensity, float diffuseIntensity);

// Apply fog
vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd);

void main() {
	// Discard if the pixel is too far away
	if(v_dist > u_renderDistance) discard;

	vec4 color = getColor();

	color *= light(vec3(1.0, 1.0, 1.0), vec4(0.0, 48.0, 0.0, 1.0), 0.5, 0.5);

	color = fog(color, v_dist, u_renderDistance - 8, u_renderDistance);

	gl_FragColor = color;
}

