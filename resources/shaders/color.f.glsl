#version 120

varying vec4 v_color;
varying vec2 v_texCoord;

uniform sampler2D u_tex;

vec4 getColor() {
	if (v_texCoord.x != -1 && v_texCoord.y != -1) {
		return texture2D(u_tex, v_texCoord);
	}

	return v_color;
}
