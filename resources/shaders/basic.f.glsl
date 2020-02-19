#version 120

varying vec4 v_coord3d;
varying vec2 v_texCoord;
varying vec4 v_color;
varying float v_faceValue;

uniform sampler2D u_tex;

uniform int u_renderType;

void main() {
	vec4 color = v_color;

	if (v_texCoord.x != -1 && v_texCoord.y != -1) {
		color = texture2D(u_tex, v_texCoord);

		if (color.r == color.g && color.g == color.b) {
			color *= v_color;
		}
	}

	// if (u_renderType == -1) {
	// 	float averageColor = (color.r + color.g + color.b) / 3.0f;
	// 	color.rgb = vec3(averageColor, averageColor, averageColor);
	// }

	if (color.a < 0.3) discard;

	if (v_faceValue == 2) {
		// color -= vec4(0.25, 0.25, 0.25, 0.0);
		color /= 2;
		color.a = v_color.a;
	}
	else if (v_faceValue == 4) {
		// color -= vec4(0.15, 0.15, 0.15, 0.0);
		color /= 1.5;
		color.a = v_color.a;
	}

	gl_FragColor = color;
}

