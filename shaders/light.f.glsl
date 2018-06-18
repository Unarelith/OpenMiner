#version 120

varying vec4 v_coord3d;
varying vec4 v_normal;

vec4 light(vec4 color, vec3 lightColor, vec4 lightPosition, float ambientIntensity, float diffuseIntensity) {
	vec4 ambientColor = vec4(lightColor * ambientIntensity, color.w);

	vec4 lightDirection = normalize(lightPosition - v_coord3d);

	float diffuseFactor = dot(v_normal, lightDirection);

	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, color.w);
	if(diffuseFactor > 0) {
		diffuseColor = vec4(lightColor * diffuseIntensity * diffuseFactor, color.w);
	}

	return color * (ambientColor + diffuseColor);
}

