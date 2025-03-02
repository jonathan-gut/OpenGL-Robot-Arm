#version 330 core

// Input from the vertex shader
in vec3 fragColor;

// Output color
out vec4 color;

void main() {
    // Set the fragment color
    color = vec4(fragColor, 1.0);
}
