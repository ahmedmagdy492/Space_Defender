#version 330 core

in vec2 outTextureCoord;

out vec4 FragColor;

uniform sampler2D inTexture;

void main() {
	FragColor = texture(inTexture, outTextureCoord);
}