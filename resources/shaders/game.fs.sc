$input v_position, v_normal, v_color0, v_texcoord0, v_texcoord1, v_texcoord2

#include <bgfx_shader.sh>

#define v_lightValue       v_texcoord1.xy
#define v_ambientOcclusion v_texcoord2.x
#define v_blockFace        v_texcoord2.y
#define v_dist             v_texcoord2.z

SAMPLER2D(u_tex, 0);

uniform vec4 u_renderDistance;
uniform vec4 u_fogColor;
uniform vec4 u_skyColor;
uniform vec4 u_sunlightIntensity;
uniform vec4 u_mipLevel;

// Get light color
vec4 light(vec4 position, vec4 normal, vec4 color, vec3 lightColor, vec4 lightPosition, float ambientIntensity, float diffuseIntensity) {
	vec3 ambientColor = vec3(lightColor * ambientIntensity);

	vec4 lightDirection = normalize(lightPosition - position);

	float diffuseFactor = dot(normal, lightDirection);

	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	if(diffuseFactor > 0) {
		diffuseColor = vec3(lightColor * diffuseIntensity * diffuseFactor);
	}

	return color * vec4(ambientColor + diffuseColor, 1.0);
}

// Apply fog
vec4 fog(vec4 color, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);

	return mix(vec4(u_skyColor.r, u_skyColor.g, u_skyColor.b, u_fogColor.a), color, fog);
}

void main() {
	// Needed to prevent bad interpolation on some systems
	// Refer to #23 for more informations
	float blockFace = floor(v_blockFace + 0.5);
	float lightCheck = floor(v_lightValue.x + 0.5);
	vec4 normal = floor(v_normal + 0.5);

	// Discard if the pixel is too far away
	if(blockFace > -1. && v_dist > u_renderDistance.x) discard;

	// Get current pixel color and apply multiplier on grayscale textures
	vec4 color = v_color0;
	if (v_texcoord0.x > -0.99 && v_texcoord0.y > -0.99) {
		color = texture2DLod(u_tex, v_texcoord0, clamp(mix(u_mipLevel.x, u_mipLevel.y, v_dist / u_renderDistance.x), u_mipLevel.x, u_mipLevel.y));
		if (blockFace > -1 && color.r == color.g && color.g == color.b) {
			color *= v_color0;
		}
	}

	// Very cheap "transparency": don't draw pixels with a low alpha value
	if(color.a < 0.3 && blockFace > -1.) discard;

	float minBrightness = 2.0 / 16.0;
	if (lightCheck != -1.) {
		float sunlight = clamp(v_lightValue.x * u_sunlightIntensity.x, 3, 15);

		float ambientIntensity = max(max(v_lightValue.x, v_lightValue.y) / 16.0, minBrightness);
		float diffuseIntensity = max(v_lightValue.x, v_lightValue.y) / 32.0;

		// Bottom
		if (normal.z == -1)
			ambientIntensity = max(ambientIntensity * 0.6, minBrightness);
		// South or North
		else if (normal.y == 1 || normal.y == -1)
			ambientIntensity = max(ambientIntensity * 0.75, minBrightness);
		// West or East
		else if (normal.x == 1 || normal.x == -1)
			ambientIntensity = max(ambientIntensity * 0.9, minBrightness);

		float lightval = clamp(sunlight / 15.0, v_lightValue.y / 15.0, 1.0);

		color = light(v_position, v_normal, color, vec3(lightval, lightval, lightval), v_position, ambientIntensity, diffuseIntensity);
	}

	color.rgb *= v_ambientOcclusion;

	color = fog(color, v_dist, u_renderDistance.x - 32, u_renderDistance.x);

	gl_FragColor = color;
}
