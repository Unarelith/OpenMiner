$input v_position, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_colorTexture, 0);
SAMPLER2D(s_depthTexture, 1);

uniform vec4 u_effectType;
uniform vec4 u_depthFogColor;

#define u_fogDepth u_effectType.y

#if 1
void main() {
	vec4 color = texture2D(s_colorTexture, v_texcoord0).rgba;
	float depth = texture2D(s_depthTexture, v_texcoord0).r;

	// Underwater effect
	if (u_effectType.x == 1) {
		/* color.rgb = mix(color.rgb, vec3(0, 0.5, 1), clamp(pow(depth, 20.0), 0.0, 1.0)); */
		color.rgb = mix(color.rgb, u_depthFogColor.rgb, clamp(pow(depth, u_fogDepth), 0.0, 1.0));
	}

	// Grayscale
	// float average = (color.r + color.g + color.b) / 3.0; // Basic
	// float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b; // More realistic
	// color = vec4(average, average, average, 1.0);

	// Inverted colors
	// color = vec4(vec3(1.0 - texture2D(s_colorTexture, v_texcoord0)), 1.0);

	gl_FragColor = color;
}
#else
void main() {
	float offset = 1.0 / 300.0;

	vec2 offsets[9];
	offsets[0] = vec2(-offset,  offset); // top-left
	offsets[1] = vec2( 0.0f,    offset); // top-center
	offsets[2] = vec2( offset,  offset); // top-right
	offsets[3] = vec2(-offset,  0.0f);   // center-left
	offsets[4] = vec2( 0.0f,    0.0f);   // center-center
	offsets[5] = vec2( offset,  0.0f);   // center-right
	offsets[6] = vec2(-offset, -offset); // bottom-left
	offsets[7] = vec2( 0.0f,   -offset); // bottom-center
	offsets[8] = vec2( offset, -offset); // bottom-right

	// Sharpen
	mat3 kernel_sharpen = mat3(
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	// Blur
	mat3 kernel_blur = mat3(
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	// Edge-detection
	mat3 kernel_edge_detect = mat3(
		1,  1, 1,
		1, -8, 1,
		1,  1, 1
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture2D(s_colorTexture, v_texcoord0.xy + offsets[i]));
	}

	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel_sharpen[i / 3][int(mod(i, 3))];

	gl_FragColor = vec4(col, 1.0);
}
#endif
