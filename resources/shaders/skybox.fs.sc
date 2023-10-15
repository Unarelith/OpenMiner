$input v_position, v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(u_tex, 0);

uniform vec4 u_skyColor;
uniform vec4 u_starColor;

void main() {
	vec4 color = v_color0;

	if (v_texcoord0.x > -0.99 && v_texcoord0.y > -0.99) {
		color = texture2D(u_tex, v_texcoord0);
		color += u_skyColor;
	}
	else if (color.a == 0) {
		color = u_starColor;
	}

	if (color.a < 0.3) discard;

	gl_FragColor = color;
}
