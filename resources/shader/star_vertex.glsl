#version 330

// Provided by Raylib
uniform mat4 mvp;
uniform mat4 matModel;

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 FragPosition;
out vec3 FragNormal;

void main() {
	FragPosition = vec3(matModel * vec4(vertexPosition, 1.0));
	FragNormal = vertexNormal;
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}
