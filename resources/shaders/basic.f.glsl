#version 120

varying vec4 v_color;
varying vec2 v_texCoord;
varying float v_faceValue;

uniform sampler2D u_tex;

void main() {
	vec4 color = v_color;

	if (v_texCoord.x != -1 && v_texCoord.y != -1) {
		vec4 texColor = texture2D(u_tex, v_texCoord);
		color = vec4(texColor.rgb - (1 - color.rgb), min(texColor.a, color.a));
	}

	if (color.a < 0.3) discard;

	if (v_faceValue == 2)
		color -= vec4(0.2, 0.2, 0.2, 0.0);
	else if (v_faceValue == 4)
		color -= vec4(0.1, 0.1, 0.1, 0.0);

	gl_FragColor = color;
}

