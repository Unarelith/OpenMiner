#version 120

varying vec4 v_color;
varying vec4 v_coord3d;
varying vec2 v_texCoord;

uniform sampler2D u_tex;

void main() {
	vec4 color = v_color;

	if (v_texCoord.x != -1 && v_texCoord.y != -1) {
		color = texture2D(u_tex, v_texCoord);

		if (color.r == color.g && color.g == color.b) {
			color *= v_color;
		}
	}

	if (color.a < 0.3) discard;

	gl_FragColor = color;
}

