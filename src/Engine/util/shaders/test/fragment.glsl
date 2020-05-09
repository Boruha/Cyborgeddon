#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D shadow_map;

void main()
{             
    float depthValue = texture(shadow_map, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}