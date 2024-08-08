#version 330 core

in vec2 outTextureCoord;

out vec4 FragColor;

uniform sampler2D inTexture;
uniform bool isShape;
uniform vec3 shapeColor;

void main() {
    if(isShape == true) {
        FragColor = vec4(shapeColor, 1.0);
    }
    else {
        FragColor = texture(inTexture, outTextureCoord);
    }
}