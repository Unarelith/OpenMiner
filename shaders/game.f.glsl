#version 120

varying vec4 v_coord3d;
varying vec4 v_normal;
varying vec2 v_texCoord;

uniform int u_renderDistance;
uniform sampler2D u_tex;

varying float v_depth;

const vec4 lightPosition = vec4(0.0, 48.0, 0.0, 1.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

const float ambientIntensity = 0.5;
const vec4 ambientColor = vec4(lightColor * ambientIntensity, 1.0);

const float diffuseIntensity = 0.5;

const vec4 fogColor = vec4(0.196078, 0.6, 0.8, 1.0);
//const vec4 fogColor = vec4(0.6, 0.8, 1.0, 1.0);
//const float fogDensity = 0.00003;

//vec4 fog(vec4 color, vec4 fogColor, float depth, float density) {
	//float fog = exp(-pow(depth * density, 2));
vec4 fog(vec4 color, vec4 fogColor, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);
	return mix(fogColor, color, fog);
}

void main() {
	vec4 lightDirection = normalize(lightPosition - v_coord3d);
	
	float diffuseFactor = dot(v_normal, lightDirection);
	
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	if(diffuseFactor > 0) {
		diffuseColor = vec4(lightColor * diffuseIntensity * diffuseFactor, 1.0);
	}
	
	vec4 color = texture2D(u_tex, v_texCoord) * (ambientColor + diffuseColor);
	if(color.a < 0.4) discard;
	
	//float z = gl_FragCoord.z / gl_FragCoord.w;
	//float depth = gl_DepthRange.diff * z + gl_DepthRange.near + gl_DepthRange.far;
	//if(depth > u_renderDistance * 16) discard;
	
	//float fogDensity = 0.0001 / (u_renderDistance / 3);
	//float fogDensity = 0.0005;
	//float fog = clamp(exp(-pow(fogDensity * z * z, 2)), 0.0, 1.0);
	//float fog = clamp(exp(-fogDensity * z * z), 0.4, 1.0);
	
	//gl_FragColor = color;
	float z = abs(gl_FragCoord.z / gl_FragCoord.w);
	gl_FragColor = fog(color, fogColor, z, u_renderDistance - 10, u_renderDistance);
	//gl_FragColor = fog(color, fogColor, v_depth, 0.005);
}

