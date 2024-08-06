#version 330 core

in vec2 outTextureCoord;

out vec4 FragColor;

uniform sampler2D inTexture;

void main() {
    /*vec3 colorFilter = vec3(1.0, 0.5, 0.5);
    vec4 color = texture(inTexture, outTextureCoord);
    FragColor = vec4(color.rgb * colorFilter, color.a);*/

	float texOffset = 0.001; // Static texOffset value
    vec2 offsets[9] = vec2[](
        vec2(-texOffset, texOffset),  vec2(0.0, texOffset),  vec2(texOffset, texOffset),
        vec2(-texOffset, 0.0),        vec2(0.0, 0.0),        vec2(texOffset, 0.0),
        vec2(-texOffset, -texOffset), vec2(0.0, -texOffset), vec2(texOffset, -texOffset)
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(inTexture, outTextureCoord.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);
}