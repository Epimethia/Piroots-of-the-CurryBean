#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 fragColor;
out vec2 fragTexCoord;

uniform mat4 MVP;
uniform float currentTime;


void main(void) {
	gl_Position = MVP* vec4(position.x, position.y, position.z * sin((currentTime/200.0)), 1.0);
	fragColor = color;
	fragTexCoord = texCoord;
}