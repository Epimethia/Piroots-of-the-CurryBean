#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

uniform mat4 MVP;
uniform mat4 model; // Or ‘normalMatrix’
uniform float currentTime;

void main(void) {
	gl_Position = MVP * vec4(position.x, (position.y * 1.5) * sin((currentTime / 500)), position.z, 1.0);

	//Diffuse
	fragPos = vec3(model * vec4(position, 1.0f));
	fragNormal = mat3(transpose(inverse(model))) * normal;
	fragTexCoord = texCoords;
}