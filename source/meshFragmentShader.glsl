#version 330 core
//TODO: P1bTask5 - Modify shader to use position, normal and light positions to compute lighting.

// Input from the vertex shader (if needed)
in vec3 FragPos; // Fragment position
in vec3 Normal;  // Fragment normal

// Uniforms
uniform bool isSelected; // Selection flag

// Output color
out vec4 color;

void main() {
    // Default color (e.g., gray)
    vec3 objectColor = vec3(0.5, 0.5, 0.5);

    // If the object is selected, make it brighter
    if (isSelected) {
        objectColor = objectColor * 2.0; // Multiply by 2 to make it brighter
    }

    // Set the fragment color
    color = vec4(objectColor, 1.0);
}
