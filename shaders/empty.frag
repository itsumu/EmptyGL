#version 430 core

in vec3 our_color;
in vec2 texture_coordinate;

uniform sampler2D texture1;

out vec4 fragment_color;

void main() {
    fragment_color = texture(texture1, texture_coordinate);
}
