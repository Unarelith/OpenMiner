#version 120

varying vec2 v_texCoord;
varying vec2 v_coord2d;

uniform sampler2D screenTexture;

void main() {
	gl_FragColor = texture2D(screenTexture, v_texCoord);
	/* gl_FragColor = vec4(vec3(1.0 - texture2D(screenTexture, v_texCoord)), 1.0); */
}

