#version 460 core

out vec4 color;

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

void main() {
    color = mix(texture(uTexture0, texCoord), texture(uTexture1, texCoord), 0.5);
}
