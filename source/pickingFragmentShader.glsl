#version 330 core

uniform highp float objectID; // Object ID for picking

out vec4 color;

void main() {
    // Encode the object ID as red channel
    color = vec4(objectID / 255.0, 0.0, 0.0, 1.0); // Normalize ID to fit in [0, 1]
}
