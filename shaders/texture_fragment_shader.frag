#version 330 core

in vec2 outTextureCoord;

out vec4 FragColor;

uniform sampler2D inTexture;

void main() {
    /*vec3 colorFilter = vec3(1.0, 0.5, 0.5);
    vec4 color = texture(inTexture, outTextureCoord);
    FragColor = vec4(color.rgb * colorFilter, color.a);*/

    FragColor = texture(inTexture, outTextureCoord);
}