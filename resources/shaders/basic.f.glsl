#version 120

varying vec4 v_color;
varying vec2 v_texCoord;

uniform sampler2D u_tex;

void main() {
	vec4 color = v_color;

	if (color == vec4(0, 0, 0, 1)) {
		color = texture2D(u_tex, v_texCoord);
	}

	if (color.a < 0.5) discard;

	gl_FragColor = color;
}

