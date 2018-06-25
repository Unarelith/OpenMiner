#version 120

varying vec4 v_color;
varying vec2 v_texCoord;
varying float v_faceValue;

uniform sampler2D u_tex;

void main() {
	vec4 color = v_color;

	if (color == vec4(0, 0, 0, 1)) {
		color = texture2D(u_tex, v_texCoord);
	}

	if (color.a < 0.3) discard;

	if (v_faceValue == 2)
		color -= vec4(0.2, 0.2, 0.2, 0.0);
	else if (v_faceValue == 4)
		color -= vec4(0.1, 0.1, 0.1, 0.0);

	gl_FragColor = color;
}

