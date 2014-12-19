#version 120

varying vec4 v_coord3d;
varying vec4 v_normal;
varying vec2 v_texCoord;

uniform sampler2D u_tex;

void main() {
	//vec4 lightSource = vec4(0.0, 48.0, 0.0, 0.0);
	//float lightIntensity = 1.0;
	
	//vec4 surfaceToLight = normalize(lightSource - v_coord3d);
	
	//float brightness = dot(v_normal, surfaceToLight) / (length(surfaceToLight) * length(v_normal));
	//brightness = clamp(brightness, 0, 1) / 2 + 0.25;
	
	//gl_FragColor = vec4(brightness * lightIntensity * vec3(1.0, 1.0, 1.0), 1.0);
	
	vec4 lightPosition = vec4(0.0, 48.0, 0.0, 1.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec4 lightDirection = normalize(lightPosition - v_coord3d);
	
	float ambientIntensity = 0.5;
	vec4 ambientColor = vec4(lightColor * ambientIntensity, 1.0);
	
	float diffuseIntensity = 0.5;
	float diffuseFactor = dot(v_normal, lightDirection);
	
	vec4 diffuseColor;
	if(diffuseFactor > 0) {
		//diffuseColor = vec4(lightColor, 1.0) * diffuseIntensity * diffuseFactor;
		diffuseColor = vec4(lightColor * diffuseIntensity * diffuseFactor, 1.0);
		//diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	
	vec4 surfaceColor = texture2D(u_tex, v_texCoord);
	gl_FragColor = surfaceColor * (ambientColor + diffuseColor);
}

