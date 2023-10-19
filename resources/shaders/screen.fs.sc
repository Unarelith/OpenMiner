$input v_position, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_colorTexture, 0);
SAMPLER2D(s_depthTexture, 1);

uniform vec4 u_effectType;
uniform vec4 u_depthFogColor;

#define u_fogDepth u_effectType.y

void main() {
	vec4 color = texture2D(s_colorTexture, v_texcoord0).rgba;
	float depth = texture2D(s_depthTexture, v_texcoord0).r;

	// Underwater effect
	if (u_effectType.x == 1) {
		/* color.rgb = mix(color.rgb, vec3(0, 0.5, 1), clamp(pow(depth, 20.0), 0.0, 1.0)); */
		color.rgb = mix(color.rgb, u_depthFogColor.rgb, clamp(pow(depth, u_fogDepth), 0.0, 1.0));
	}

	// Grayscale
	/* float average = (color.r + color.g + color.b) / 3.0; // Basic */
	/* float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b; // More realistic */
	/* color = vec4(average, average, average, 1.0); */

	// Inverted colors
	/* color = vec4(vec3(1.0 - texture2D(s_colorTexture, v_texcoord0)), 1.0); */

	gl_FragColor = color;
}

// const float offset = 1.0 / 300.0;
//
// void main() {
// 	vec2 offsets[9] = vec2[](
// 		vec2(-offset,  offset), // top-left
// 		vec2( 0.0f,    offset), // top-center
// 		vec2( offset,  offset), // top-right
// 		vec2(-offset,  0.0f),   // center-left
// 		vec2( 0.0f,    0.0f),   // center-center
// 		vec2( offset,  0.0f),   // center-right
// 		vec2(-offset, -offset), // bottom-left
// 		vec2( 0.0f,   -offset), // bottom-center
// 		vec2( offset, -offset)  // bottom-right
// 	);
//
// 	// Sharpen
// 	/* float kernel[9] = float[]( */
// 	/* 	-1, -1, -1, */
// 	/* 	-1,  9, -1, */
// 	/* 	-1, -1, -1 */
// 	/* ); */
//
// 	// Blur
// 	/* float kernel[9] = float[]( */
// 	/* 	1.0 / 16, 2.0 / 16, 1.0 / 16, */
// 	/* 	2.0 / 16, 4.0 / 16, 2.0 / 16, */
// 	/* 	1.0 / 16, 2.0 / 16, 1.0 / 16 */
// 	/* ); */
//
// 	// Edge-detection
// 	/* float kernel[9] = float[]( */
// 	/* 	1,  1, 1, */
// 	/* 	1, -8, 1, */
// 	/* 	1,  1, 1 */
// 	/* ); */
//
// 	vec3 sampleTex[9];
// 	for(int i = 0; i < 9; i++) {
// 		sampleTex[i] = vec3(texture2D(s_colorTexture, v_texcoord0.st + offsets[i]));
// 	}
//
// 	vec3 col = vec3(0.0);
// 	for(int i = 0; i < 9; i++)
// 		col += sampleTex[i] * kernel[i];
//
// 	gl_FragColor = vec4(col, 1.0);
// }
