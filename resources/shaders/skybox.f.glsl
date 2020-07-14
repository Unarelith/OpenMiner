#version 120

varying vec4 v_color;
varying vec4 v_coord3d;
varying vec2 v_texCoord;

uniform sampler2D u_tex;

uniform vec4 u_skyColor;

void main() {
	vec4 color = v_color;

	if (v_texCoord.x > -0.99 && v_texCoord.y > -0.99) {
		color = texture2D(u_tex, v_texCoord);

		color += u_skyColor;
	}

	if (color.a < 0.3) discard;

	gl_FragColor = color;
}

