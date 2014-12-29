#version 120

varying vec4 v_color;
varying vec4 v_coord3d;
varying vec4 v_normal;
varying vec2 v_texCoord;

varying vec4 v_viewSpace;

uniform int u_renderDistance;
uniform sampler2D u_tex;

const vec4 lightPosition = vec4(0.0, 48.0, 0.0, 1.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

const float ambientIntensity = 0.5;
const vec4 ambientColor = vec4(lightColor * ambientIntensity, 1.0);

const float diffuseIntensity = 0.5;

const vec4 fogColor = vec4(0.196078, 0.6, 0.8, 1.0);
//const vec4 fogColor = vec4(0.6, 0.8, 1.0, 1.0);

vec4 fog(vec4 color, vec4 fogColor, float fogCoord, float fogStart, float fogEnd);

void main() {
	vec4 lightDirection = normalize(lightPosition - v_coord3d);
	
	float diffuseFactor = dot(v_normal, lightDirection);
	
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	if(diffuseFactor > 0) {
		diffuseColor = vec4(lightColor * diffuseIntensity * diffuseFactor, 1.0);
	}
	
	vec4 color = v_color;
	if(color == vec4(0, 0, 0, 1)) {
		color = texture2D(u_tex, v_texCoord) * (ambientColor + diffuseColor);
	}
	
	if(color.a < 0.4) discard;
	
	float dist = length(v_viewSpace);
	if(dist > u_renderDistance) discard;
	
	gl_FragColor = fog(color, fogColor, dist, u_renderDistance - 4, u_renderDistance);
}

vec4 fog(vec4 color, vec4 fogColor, float fogCoord, float fogStart, float fogEnd) {
	float fog = clamp((fogEnd - fogCoord) / (fogEnd - fogStart), 0.0, 1.0);
	return mix(fogColor, color, fog);
}

