#include "meshObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <common/objloader.hpp>


// Initialize static member
int meshObject::nextId = 1;
std::map<int, meshObject*> meshObject::meshObjectMap;

// TODO: P1aTask3 - Modify the constructor of Object to accept the path to an obj file
meshObject::meshObject(const std::string &objFilePath) : id(nextId++) { // Assign current value of nextId to id and increment it

    // Add this object to the map
    meshObjectMap[id] = this;

    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);

    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // TODO: P1aTask3 - Read from obj file instead.
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    loadOBJ(objFilePath, vertices, indices);

    
    //TODO: P1bTask5 - Create normal buffer.
    
    numIndices = (GLsizei)indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // Normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load and compile shaders
    shaderProgram = LoadShaders("meshVertexShader.glsl", "meshFragmentShader.glsl");
    pickingShaderProgram = LoadShaders("pickingVertexShader.glsl", "pickingFragmentShader.glsl");
}

meshObject::~meshObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    // Remove this object from the map
    meshObjectMap.erase(id);
}

//TODO: P1bTask5 - Modify to accept lighiting info as arguement.
void meshObject::draw(const glm::mat4& parentTransform, const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    
    // Compute the global transformation matrix for this object
    glm::mat4 globalTransform = parentTransform * modelMatrix;
    
    // Compute the MVP matrix
    glm::mat4 MVP = projection * view * globalTransform;
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));
    
    // Pass the selection flag to the shader
    GLuint isSelectedID = glGetUniformLocation(shaderProgram, "isSelected");
    glUniform1i(isSelectedID, isSelected);
    
    // Draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    // Recursively call for children, passing the global transformation
    for (meshObject* child : children) {
        child->draw(globalTransform, view, projection);
    }
}

void meshObject::translate(const glm::vec3& translation) {
    // Apply translation to the model matrix
    modelMatrix = glm::translate(modelMatrix, translation) ;
    

}

void meshObject::rotate(float angle, const glm::vec3& axis) {
    // Apply the rotation to the model matrix
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);

    // Recursively rotate all children
    for (meshObject* child : children) {
        child->rotate(angle, axis);
    }
}

void meshObject::drawPicking(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(pickingShaderProgram); // Use the picking shader

    //TODO: P1bBonus2 - Send the value of the id variable to the shader as a uniform. Use glUniform1f

    glm::mat4 MVP = projection * view * modelMatrix;
    GLuint matrixID = glGetUniformLocation(pickingShaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

    // Draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0); // Unbind the shader program
}

meshObject* meshObject::getMeshObjectById(int id) {
    // Look up the object by ID in the map
    auto it = meshObjectMap.find(id);
    if (it != meshObjectMap.end()) {
        return it->second;
    }
    return nullptr; // Return nullptr if ID not found
}

// Helper function to laod OBJ files
void meshObject::loadOBJ(const std::string& filePath, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        return;
    } else {
        std::cout << "Opened OBJ file: " << filePath << std::endl;
    }

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<GLuint> tempIndices;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") { // Vertex position
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        } else if (type == "vn") { // Vertex normal
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        } else if (type == "f") { // Face
            unsigned int v1, v2, v3, n1, n2, n3;
            char slash;
            ss >> v1 >> slash >> slash >> n1
               >> v2 >> slash >> slash >> n2
               >> v3 >> slash >> slash >> n3;
            tempIndices.push_back(v1 - 1);
            tempIndices.push_back(v2 - 1);
            tempIndices.push_back(v3 - 1);
        }
    }

    // Combine vertices and normals into a single vertex array
    for (size_t i = 0; i < tempVertices.size(); i++) {
        vertices.push_back(tempVertices[i].x);
        vertices.push_back(tempVertices[i].y);
        vertices.push_back(tempVertices[i].z);

        if (i < tempNormals.size()) {
            vertices.push_back(tempNormals[i].x);
            vertices.push_back(tempNormals[i].y);
            vertices.push_back(tempNormals[i].z);
        } else {
            // If no normals are provided, use a default normal (e.g., (0, 1, 0))
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);
        }
    }
    indices = tempIndices;
}

void meshObject::setSelected(bool flag) {
    isSelected = flag;
}
