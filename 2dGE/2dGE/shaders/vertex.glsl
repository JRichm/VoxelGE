#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 instanceColor;
layout (location = 2) in mat4 model;

out vec3 color;

void main() {
    gl_Position = model * vec4(aPos, 1.0);
    color = instanceColor;
}