$input v_color0, v_color1, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(u_tex, 0);

// uniform int u_renderType;

void main() {
	vec4 color = v_color0;

	if (v_texcoord0.x != -1 && v_texcoord0.y != -1) {
		color = texture2D(u_tex, v_texcoord0);

		if (color.r == color.g && color.g == color.b) {
			color *= v_color0;
		}
	}

	// if (u_renderType == -1) {
	// 	float averageColor = (color.r + color.g + color.b) / 3.0f;
	// 	color.rgb = vec3(averageColor, averageColor, averageColor);
	// }

	if (color.a < 0.3) discard;

#ifdef OM_NOT_IMPLEMENTED
	if (v_color1 == 2) {
		// color -= vec4(0.25, 0.25, 0.25, 0.0);
		color.rgb /= 2.3;
		color.a = v_color0.a;
	}
	else if (v_color1 == 4) {
		// color -= vec4(0.15, 0.15, 0.15, 0.0);
		color.rgb /= 1.6;
		color.a = v_color0.a;
	}
#endif // OM_NOT_IMPLEMENTED

	gl_FragColor = color;
}

