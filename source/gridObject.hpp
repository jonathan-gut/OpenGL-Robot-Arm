#ifndef gridObject_hpp
#define gridObject_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <common/shader.hpp>

class gridObject {
public:
    gridObject();
    ~gridObject();

    void draw(const glm::mat4& view, const glm::mat4& projection);


private:
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    glm::mat4 modelMatrix;

    
    GLsizei numIndices;
    
};

#endif
