#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoords;

out vec3 fragColor;
out vec2 fragTexCoord;

uniform mat4 MVP;
uniform float currentTime;


void main(void) {
	gl_Position = MVP* vec4(position.x, position.y * sin((currentTime / 500)), position.z, 1.0);
	fragColor = color;
	fragTexCoord = texCoords;
}