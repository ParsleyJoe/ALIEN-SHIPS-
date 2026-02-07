#version 330

uniform vec3 starColor;
uniform vec3 starPosition;

out vec4 FragColor;

in vec3 FragPosition;
in vec3 FragNormal;

void main(){
	vec3 lightPosition = starPosition;
	vec3 lightAmbient = vec3(0.6, 0.6, 0.6);
	vec3 lightDiffuse = vec3(1.0, 1.0, 1.0);
	vec3 starAmbient = vec3(1.0, 1.0, 1.0);
	vec3 starDiffuse = vec3(0.5, 0.5, 0.5);

	vec3 ambient = lightAmbient * starAmbient;

	vec3 lightDirection = normalize(FragPosition - lightPosition);
	float diff = max(dot(FragNormal, -lightDirection), 0);
	vec3 diffuse = diff * lightDiffuse * starDiffuse;
	FragColor = vec4((ambient + diffuse) * starColor, 1.0);
}
