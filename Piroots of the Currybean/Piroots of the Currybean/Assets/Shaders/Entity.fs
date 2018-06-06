#version 450 core
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 color;
uniform sampler2D tex;

uniform float ambientStr = 0.5f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos = vec3(0.0f, 13.0f, 0.0f);

void main() {

    vec3 ambient = ambientStr * ambientColor;
    // Light Direction
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(fragPos - lightPos);
    // Diffuse Coloring
    float diffuseStr = max(dot(norm, -lightDir ), 0.0f);
    vec3 diffuse = diffuseStr * lightColor;

    //color = vec4(ambient, 1.0f) * texture(tex, fragTexCoord);
    color = vec4(ambient + diffuse, 1.0f) * texture(tex, fragTexCoord);
}

