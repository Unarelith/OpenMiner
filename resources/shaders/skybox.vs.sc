$input a_position, a_color0, a_texcoord0
$output v_position, v_color0, v_texcoord0

#include <bgfx_shader.sh>

void main() {
	v_position = mul(u_model[0], vec4(a_position.xyz, 1.0));
	v_color0 = a_color0;
	v_texcoord0 = a_texcoord0;

	gl_Position = mul(u_viewProj, v_position);
}
