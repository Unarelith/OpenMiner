$input a_position, a_normal, a_color0, a_texcoord0, a_texcoord1, a_texcoord2
$output v_position, v_normal, v_color0, v_texcoord0, v_texcoord1, v_texcoord2

#include <bgfx_shader.sh>

#define a_lightValue       a_texcoord1.xy
#define a_ambientOcclusion a_texcoord2.x

#define v_lightValue       v_texcoord1.xy
#define v_ambientOcclusion v_texcoord2.x
#define v_blockFace        v_texcoord2.y
#define v_dist             v_texcoord2.z

void main() {
	// Used for lighting
	v_position = u_model[0] * vec4(a_position.xyz, 1.0);
	v_normal = vec4(a_normal, 1.0);

	v_color0 = a_color0;
	v_texcoord0 = a_texcoord0;
	v_lightValue = a_lightValue;

	if (a_ambientOcclusion != 4) {
		const vec4 aovalues = vec4(0.4, 0.6, 0.8, 1.0);
		v_ambientOcclusion = aovalues[int(a_ambientOcclusion)];
	} else {
		v_ambientOcclusion = 1.0;
	}

	v_blockFace = a_position.w;

	// Distance from eye
	v_dist = length(u_view * v_position);

	gl_Position = u_proj * u_view * v_position;
}
