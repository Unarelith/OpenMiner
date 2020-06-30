#version 120

varying vec2 v_texCoord;
varying vec2 v_coord2d;

uniform sampler2D colorTexture;
uniform sampler2D bloomTexture;
uniform sampler2D depthTexture;

uniform int u_effectType;
uniform float u_fogDepth;
uniform vec4 u_fogColor;

void main() {
	vec4 color = texture2D(colorTexture, v_texCoord);
	vec4 bloom = texture2D(bloomTexture, v_texCoord);
	float depth = texture2D(depthTexture, v_texCoord).r;

	// Underwater effect
	if (u_effectType == 1) {
		/* color.rgb = mix(color.rgb, vec3(0, 0.5, 1), clamp(pow(depth, 20.0), 0.0, 1.0)); */
		color.rgb = mix(color.rgb, u_fogColor.rgb, clamp(pow(depth, u_fogDepth), 0.0, 1.0));
	}

	/* if (gl_FragCoord.x < 800) */
		color.rgb += bloom.rgb;

	// Grayscale
	/* float average = (color.r + color.g + color.b) / 3.0; // Basic */
	/* float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b; // More realistic */
	/* color = vec4(average, average, average, 1.0); */

	// Inverted colors
	/* color = vec4(vec3(1.0 - texture2D(screenTexture, v_texCoord)), 1.0); */

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
// 		sampleTex[i] = vec3(texture2D(screenTexture, v_texCoord.st + offsets[i]));
// 	}
//
// 	vec3 col = vec3(0.0);
// 	for(int i = 0; i < 9; i++)
// 		col += sampleTex[i] * kernel[i];
//
// 	gl_FragColor = vec4(col, 1.0);
// }
