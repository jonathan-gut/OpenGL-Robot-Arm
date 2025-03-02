#version 330 core

// Input vertex attributes (from VBO)
layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec3 color;    // Vertex color

// Uniforms
uniform mat4 MVP; // Combined Model-View-Projection matrix

void main() {
    // Transform the vertex position
    gl_Position = MVP * vec4(position, 1.0);

}
