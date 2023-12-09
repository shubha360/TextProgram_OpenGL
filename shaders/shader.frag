#version 130

in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D textSampler;

void main() {
    vec4 textColor = texture(textSampler, fragmentUV);

    color = textColor;
}