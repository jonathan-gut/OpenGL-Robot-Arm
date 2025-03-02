#include "gridObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


gridObject::gridObject() { // Assign current value of nextId to id and increment it
    
    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);

    // Generate and bind VAO, VBO, and EBO
    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    std::vector<GLfloat> vertices;

    // Grid lines along X-axis
    for (int i = -5; i <= 5; i++) {
        vertices.insert(vertices.end(), {
            -5.0f, 0.0f, (float)i,  0.5f, 0.5f, 0.5f, 
             5.0f, 0.0f, (float)i,  0.5f, 0.5f, 0.5f
        });
    }

    // Grid lines along Z-axis
    for (int i = -5; i <= 5; i++) {
        vertices.insert(vertices.end(), {
            (float)i, 0.0f, -5.0f,  0.5f, 0.5f, 0.5f,
            (float)i, 0.0f,  5.0f,  0.5f, 0.5f, 0.5f
        });
    }

    // X-axis (Red)
    vertices.insert(vertices.end(), {
         0.0f, 0.01f,  0.0f,   1.0f, 0.0f, 0.0f,  //  (Slightly above grid to make it visible over grey)
         5.0f, 0.01f,  0.0f,   1.0f, 0.0f, 0.0f
    });


    // Y-axis (Green)
    vertices.insert(vertices.end(), {
         0.0f, 0.0f,  0.0f,   0.0f, 1.0f, 0.0f,
         0.0f, 5.0f,  0.0f,   0.0f, 1.0f, 0.0f
    });

    // Z-axis (Blue)
    vertices.insert(vertices.end(), {
         0.0f, 0.01f,  0.0f,   0.0f, 0.0f, 1.0f,  //  (Slightly above grid to make it visible over grey)
         0.0f, 0.0f,  5.0f,   0.0f, 0.0f, 1.0f
    });
    
    std::vector<GLuint> indices;
    for (GLuint i = 0; i < vertices.size() / 6; i += 2) {
        indices.push_back(i);
        indices.push_back(i + 1);
    }


    
    numIndices = (GLsizei)indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load and compile shaders
    shaderProgram = LoadShaders("gridVertexShader.glsl", "gridFragmentShader.glsl");
}

gridObject::~gridObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
}

void gridObject::draw(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);

    // Compute the Model-View-Projection (MVP) matrix
    glm::mat4 MVP = projection * view * modelMatrix;

    // Get the location of the "MVP" uniform in the shader and pass the computed matrix
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));

    // Bind the Vertex Array Object (VAO)
    glBindVertexArray(VAO);

    // Draw the grid and axis lines
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);

    // Unbind the VAO to avoid accidental modification
    glBindVertexArray(0);
}

