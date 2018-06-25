#version 120

varying vec4 v_coord3d;
varying vec4 v_normal;

vec4 light(vec4 color, vec3 lightColor, vec4 lightPosition, float ambientIntensity, float diffuseIntensity) {
	vec3 ambientColor = vec3(lightColor * ambientIntensity);

	vec4 lightDirection = normalize(lightPosition - v_coord3d);

	float diffuseFactor = dot(v_normal, lightDirection);

	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	if(diffuseFactor > 0) {
		diffuseColor = vec3(lightColor * diffuseIntensity * diffuseFactor);
	}

	return color * vec4(ambientColor + diffuseColor, 1.0);
}

