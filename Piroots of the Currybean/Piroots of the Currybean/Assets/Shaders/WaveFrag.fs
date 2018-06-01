#version 450 core
in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;
uniform sampler2D tex;

void main() {
    color = vec4(0.1, 0.6, 1.0, 0.5);//texture(tex, fragTexCoord); // 
}