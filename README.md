# OpenGL Robot Arm Simulation


## Overview
This project is a **3D Robot Arm Simulation** built using **OpenGL** and **C++**. It demonstrates my ability to work with 3D graphics, hierarchical transformations, and lighting in a real-time rendering environment. The robot arm consists of multiple parts (base, arm1, joint, arm2) that can be manipulated using keyboard inputs, showcasing my understanding of **affine transformations**, **camera control**, and **lighting models**.

This project was completed as part of a coursework assignment, and I extended it to include advanced features like **hierarchical transformations**, **lighting**, and **object selection highlighting**.

---

## Features
- **Hierarchical Transformations**: The robot arm parts are connected hierarchically, allowing realistic movement (e.g., rotating the base affects all connected parts).
- **Camera Control**: The camera can be rotated around the scene using keyboard inputs, providing a dynamic view of the robot arm.
- **Lighting**: Two light sources are positioned near the camera, illuminating the scene with **diffuse**, **ambient**, and **specular** lighting.
- **Object Selection**: The selected part of the robot arm is highlighted in a brighter color, making it easy to identify which part is being manipulated.
- **Keyboard Inputs**:
  - **Base Translation**: Move the base along the XZ plane.
  - **Base Rotation**: Rotate the base around the Y-axis.
  - **Arm1 Rotation**: Rotate Arm1 up and down.
  - **Arm2 Rotation**: Rotate Arm2 up and down.

---

## Technologies Used
- **OpenGL**: For 3D rendering and transformations.
- **GLM**: For matrix and vector math.
- **GLFW**: For window management and input handling.
- **GLSL**: For writing vertex and fragment shaders.
- **Blender**: For creating and exporting 3D models in `.obj` format.

---
