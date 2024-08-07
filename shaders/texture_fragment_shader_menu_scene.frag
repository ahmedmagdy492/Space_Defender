#version 330 core

in vec2 outTextureCoord;

out vec4 FragColor;

uniform sampler2D inTexture;

void main() {
    vec3 colorFilter = vec3(0.0, 0.5, 1.0);
    vec4 color = texture(inTexture, outTextureCoord);
    FragColor = vec4(color.rgb * colorFilter, color.a);
}