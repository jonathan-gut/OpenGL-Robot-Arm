// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "meshObject.hpp"
#include "gridObject.hpp"

// Function prototypes
int initWindow(void);
static void mouseCallback(GLFWwindow*, int, int, int);
int getPickedId();


const GLuint windowWidth = 1024, windowHeight = 768;
GLFWwindow* window;

// The object that is currently selected for P1bTask4
int currSelected = 0;


int main() {
    // ATTN: REFER TO https://learnopengl.com/Getting-started/Creating-a-window
    // AND https://learnopengl.com/Getting-started/Hello-Window to familiarize yourself with the initialization of a window in OpenGL
    
    if (initWindow() != 0) return -1;

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for Project 2, use an ortho camera :
    // gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

    
    //TODO: P1aTask3 - Create each robot arm piece by creating instances class meshObject.
    gridObject grid;
    meshObject base("../base.obj"); // Load the base
    meshObject arm1("../arm1.obj"); // Load arm1
    meshObject joint("../joint.obj"); // Load the joint
    meshObject arm2("../arm2.obj"); // Load arm2
    
    // TODO: P1aTask3 - Translate each robot arm piece to its appropriate location
    base.translate(glm::vec3(0, 0, 0));      // Base at the origin
    
    
    arm1.translate(glm::vec3(0, 0.5, 0));  // Upper arm above base
    arm1.rotate(25.0f,glm::vec3(1,0,0)); // rotate arm1
    
    joint.translate(glm::vec3(0, 1.3, 0.4));   // joint above upper arm

    arm2.translate(glm::vec3(0, 1.3, 0.5));   // arm2 at the end
    arm2.rotate(80.0f,glm::vec3(1,0,0));        // rotate arm2
    
    // TODO: P1bTask4 - Create a hierarchical structure and adjust the relative translations.
    base.children.push_back(&arm1);
    arm1.children.push_back(&joint);
    joint.children.push_back(&arm2);

    //TODO: P1aTask2 - Create variables to keep track of camera angles.
    float camTheta = glm::radians(45.0f); //horizontal angle around y axis
    float camPhi = glm::radians(30.0f); //vertical angle (elevation)
    float camRadius = 10.0f; // distance from origin
    
    //TODO: P1bTask5 - Create variables to store lighting info.
    
    
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    do {
        
        // Timing
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }
        
        //TODO: P1aTask2 - set currSelected to 0 when key c is pressed.
        
        //Reset camera when C is pressed
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            currSelected = 0;
        }

        
        //TODO: P1aTask2 - Add the cases for movment along the other axis.
        float speed = 1.5f * (1.0f/60.f); // Adjust speed based on frame rate, assuming 60 FPS, which is 1/60 seconds per frame
        
        if (currSelected == 0) {
            // Rotate left and right
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                camTheta -= speed;  // Rotate left (negative theta)
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                camTheta += speed;  // Rotate right (positive theta)
            }

            // Rotate up  and down
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                camPhi += speed;  // Move up
                if (camPhi > glm::radians(89.0f)) camPhi = glm::radians(89.0f); // Limit to avoid flipping
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                camPhi -= speed;  // Move down
                if (camPhi < glm::radians(-89.0f)) camPhi = glm::radians(-89.0f); // Limit to avoid flipping
            }
            
        }

        
        //TODO: P1aTask2 - Create the view matrix based on camera angles.  
        
        // Compute camera position using spherical coordinates - ensures camera always looks at origin (0,0,0)
        float camX = camRadius * cos(camPhi) * sin(camTheta);
        float camY = camRadius * sin(camPhi);
        float camZ = camRadius * cos(camPhi) * cos(camTheta);

        // Update view matrix
        glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(camX, camY, camZ),  // Camera position
            glm::vec3(0.0f, 0.0f, 0.0f),  // Look at the origin
            glm::vec3(0.0f, 1.0f, 0.0f)   // Up direction
        );


        // Draw picking for P1bBonus2
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //TODO: P1bBonus2 draw all robort arm pieces using drawPicking function
            //obj.drawPicking(viewMatrix, projectionMatrix);
            currSelected = getPickedId();
            
            std::cout << "Picked id: " << currSelected << std::endl;
            
            //TODO: P1bBonus2 - meshObject::getMeshObjectById can be used to get the picked object.
        }
        
        //TODO: P1bTask4 - On key press set currSelected to the id of the robot piece to select.
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            std::cout << "Picked Base Translation" << std::endl;
            currSelected = 1; // Base (translate)
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            std::cout << "Picked Base Rotation" << std::endl;
            currSelected = 2; // Base (rotate)
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            std::cout << "Picked Arm 1" << std::endl;
            currSelected = 3; // Arm1 (rotate)
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            std::cout << "Picked Arm 2" << std::endl;
            currSelected = 4; // Arm2 (rotate)
        }

        
        //TODO: P1bTask4 - On key press, based on currSelected, make appropriate transformation.
        float moveSpeed = 0.05f;
        float rotateSpeed = 2.0f;

        if (currSelected == 1) { // Base Translation (XZ Plane)
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                base.translate(glm::vec3(0, 0, -moveSpeed)); // Move forward

            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                base.translate(glm::vec3(0, 0, moveSpeed)); // Move backward

                
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                base.translate(glm::vec3(-moveSpeed, 0, 0)); // Move left

            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                base.translate(glm::vec3(moveSpeed, 0, 0)); // Move right

            }
        }

        if (currSelected ==2) { // Base Rotation (Rotates entire model around Y-axis)
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                base.rotate(rotateSpeed, glm::vec3(0, 1, 0));
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                base.rotate(-rotateSpeed, glm::vec3(0, 1, 0));
            }
        }

        if (currSelected == 3) { // Arm1 Rotation (Up/down)
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                arm1.rotate(rotateSpeed, glm::vec3(1, 0, 0)); // Rotate upwards
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                arm1.rotate(-rotateSpeed, glm::vec3(1, 0, 0)); // Rotate downwards
            }
        }

        if (currSelected == 4) { // Arm2 Rotation (Up/down)
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                arm2.rotate(rotateSpeed, glm::vec3(1, 0, 0)); // Rotate upwards
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                arm2.rotate(-rotateSpeed, glm::vec3(1, 0, 0)); // Rotate downwards
            }
        }

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // DRAWING the SCENE
        
        //TODO: P1aTask3 - Draw all robot arm pieces.
        /*
        grid.draw(viewMatrix, projectionMatrix);
        base.draw(viewMatrix, projectionMatrix);
        arm1.draw(viewMatrix, projectionMatrix);
        joint.draw(viewMatrix, projectionMatrix);
        arm2.draw(viewMatrix, projectionMatrix);
        */
        
        //TODO: P1bTask4 - Draw the robot arm pieces using the hierachy instead. Call the draw function on the root node. The remeaining pieces will be drawn using recursive calls.
        grid.draw(viewMatrix, projectionMatrix);
        base.draw(glm::mat4(1.0f), viewMatrix, projectionMatrix); 

        //TODO: P1bTask5 - Pass the lighting info to the draw function.
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}

// Initialize GLFW and create a window
int initWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // FOR MAC

    window = glfwCreateWindow(windowWidth, windowHeight, "Gutierrez,Jonathan(4977-3671)", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Set up inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    glfwSetMouseButtonCallback(window, mouseCallback);
    
    // Dark gray background (changed from blue to make lines easier to see)
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glDisable(GL_CULL_FACE);
    
    return 0;
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left mouse button pressed" << std::endl;
    }
}

int getPickedId(){
    glFlush();
    // --- Wait until all the pending drawing commands are really done.
    // Ultra-mega-over slow !
    // There are usually a long time between glDrawElements() and
    // all the fragments completely rasterized.
    glFinish();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    unsigned char data[4];

    //TODO: P1bBonus2 - Use glfwGetCursorPos to get the x and y value of the cursor.
    
    //TODO: P1bBonus2 - Use glfwGetFramebufferSize and glfwGetWindowSize to get the frame buffer size and window size. On high resolution displays, these sizes might be different.
    
    
    //TODO: P1bBonus2 - Use glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data) to read the pixel data.
    // Note that y position has to be flipped as glfwGetCursorPos gives the cursor position relative to top left of the screen. The read location must also be multiplied by (buffer size / windowSize) for some displays.
    
    int pickedId = data[0];
    return pickedId;
}
