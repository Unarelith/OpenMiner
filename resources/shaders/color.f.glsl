#version 120

varying vec4 v_color;
varying vec2 v_texCoord;

varying float v_blockID;

uniform sampler2D u_tex;

vec4 getColor() {
	vec4 color = v_color;

	// if(color == vec4(0, 0, 0, 1)) {
	if (v_texCoord.x != -1 && v_texCoord.y != -1) {
		vec4 texColor = texture2D(u_tex, v_texCoord);
		color = vec4(texColor.rgb - (1 - color.rgb), min(texColor.a, color.a));
	}

	return color;
}
