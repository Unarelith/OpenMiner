#version 120

varying vec4 v_color;
varying vec2 v_texCoord;

varying float v_blockID;

uniform sampler2D u_tex;

vec4 getColor() {
	vec4 color = v_color;

	if(color == vec4(0, 0, 0, 1)) {
		color = texture2D(u_tex, v_texCoord);
	}

	// Very cheap "transparency": don't draw pixels with a low alpha value
	if(color.a < 0.3 && v_blockID != -1) discard;

	return color;
}
