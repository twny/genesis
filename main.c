// main.c i=i+1  C baby C!!!

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

// Vertex shader source code
const char* vertexShaderSource = "#version 120\n"
    "attribute vec3 position;\n"
    "attribute vec3 color;\n"
    "varying vec3 fragColor;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main() {\n"
    "   gl_Position = projection * view * model * vec4(position, 1.0);\n"
    "   fragColor = color;\n"
    "}\0";

// Fragment shader source code
const char* fragmentShaderSource = "#version 120\n"
    "varying vec3 fragColor;\n"
    "void main() {\n"
    "   gl_FragColor = vec4(fragColor, 1.0);\n"
    "}\n\0";

// Dot position
float dotX = 0.0f;
float dotY = 0.0f;
float dotZ = 0.0f;

// Camera variables
vec3 cameraPos = {0.0f, 5.0f, 10.0f};
vec3 cameraFront = {0.0f, -0.5f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};

// Projection matrix
mat4 projection;

// Callback for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Adjust the viewport
    glViewport(0, 0, width, height);

    // Recalculate the projection matrix
    float ratio = (float)width / (float)height;
    glm_perspective(glm_rad(45.0f), ratio, 0.1f, 100.0f, projection);
}

void processInput(GLFWwindow *window) {
    const float moveSpeed = 0.1f;
    int movement = 0;

    // Dot movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        dotZ -= moveSpeed;
        movement = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        dotZ += moveSpeed;
        movement = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        dotX -= moveSpeed;
        movement = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        dotX += moveSpeed;
        movement = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        dotY += moveSpeed;
        movement = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        dotY -= moveSpeed;
        movement = 1;
    }

    // Print coordinates when they change
    if (movement) {
        printf("Dot Position: X=%.2f, Y=%.2f, Z=%.2f\n", dotX, dotY, dotZ);
    }

    // Camera movement
    const float cameraSpeed = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraPos[0] += cameraFront[0] * cameraSpeed;
        cameraPos[1] += cameraFront[1] * cameraSpeed;
        cameraPos[2] += cameraFront[2] * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraPos[0] -= cameraFront[0] * cameraSpeed;
        cameraPos[1] -= cameraFront[1] * cameraSpeed;
        cameraPos[2] -= cameraFront[2] * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        vec3 right;
        glm_cross(cameraFront, cameraUp, right);
        glm_normalize(right);
        cameraPos[0] -= right[0] * cameraSpeed;
        cameraPos[1] -= right[1] * cameraSpeed;
        cameraPos[2] -= right[2] * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        vec3 right;
        glm_cross(cameraFront, cameraUp, right);
        glm_normalize(right);
        cameraPos[0] += right[0] * cameraSpeed;
        cameraPos[1] += right[1] * cameraSpeed;
        cameraPos[2] += right[2] * cameraSpeed;
    }
}

void generateGrid(float size, int divisions, GLfloat** vertices, int* vertexCount) {
    int lineCount = (divisions + 1) * 4;
    *vertexCount = lineCount * 3 * 2; // 3 components per vertex, 2 vertices per line
    *vertices = malloc((*vertexCount) * sizeof(GLfloat));

    float halfSize = size / 2.0f;
    float divisionSize = size / divisions;
    int index = 0;

    for (int i = 0; i <= divisions; ++i) {
        float coord = -halfSize + i * divisionSize;

        // Lines parallel to X-axis (along Z)
        (*vertices)[index++] = -halfSize; // x1
        (*vertices)[index++] = 0.0f;      // y1
        (*vertices)[index++] = coord;     // z1

        (*vertices)[index++] = halfSize;  // x2
        (*vertices)[index++] = 0.0f;      // y2
        (*vertices)[index++] = coord;     // z2

        // Lines parallel to Z-axis (along X)
        (*vertices)[index++] = coord;     // x1
        (*vertices)[index++] = 0.0f;      // y1
        (*vertices)[index++] = -halfSize; // z1

        (*vertices)[index++] = coord;     // x2
        (*vertices)[index++] = 0.0f;      // y2
        (*vertices)[index++] = halfSize;  // z2
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Error initializing GLFW\n");
        return -1;
    }

    // Set OpenGL version (legacy for simplicity)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple 3D Engine", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Error creating window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the framebuffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW (needs to be done after making context current)
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
        return -1;
    }

    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check vertex shader
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex Shader Compilation Failed. InfoLog:\n%s\n", infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Fragment Shader Compilation Failed. InfoLog:\n%s\n", infoLog);
    }

    // Link shaders to create a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindAttribLocation(shaderProgram, 0, "position");
    glBindAttribLocation(shaderProgram, 1, "color");
    glLinkProgram(shaderProgram);

    // Check shader program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader Program Linking Failed. InfoLog:\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Get uniform locations
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");

    // Define the dot (as a point)
    GLfloat dotVertices[] = {
        // Positions    // Colors
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f  // Red color
    };

    // Generate VAO and VBO for the dot
    GLuint dotVAO, dotVBO;
    glGenVertexArrays(1, &dotVAO);
    glGenBuffers(1, &dotVBO);

    // Bind VAO for the dot
    glBindVertexArray(dotVAO);

    // Bind VBO and upload data
    glBindBuffer(GL_ARRAY_BUFFER, dotVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dotVertices), dotVertices, GL_STATIC_DRAW);

    // Position attribute
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Color attribute
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unbind VAO
    glBindVertexArray(0);

    // Define axes vertices
    GLfloat axesVertices[] = {
        // Positions          // Colors
        // X-axis (Red)
        -10.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         10.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        // Y-axis (Green)
        0.0f, -10.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f,  10.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        // Z-axis (Blue)
        0.0f, 0.0f, -10.0f,   0.0f, 0.0f, 1.0f,
        0.0f, 0.0f,  10.0f,   0.0f, 0.0f, 1.0f,
    };

    // Generate VAO and VBO for axes
    GLuint axesVAO, axesVBO;
    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);

    glBindVertexArray(axesVAO);

    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Color attribute
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    // Generate grid vertices
    GLfloat* gridVertices;
    int gridVertexCount;
    generateGrid(20.0f, 20, &gridVertices, &gridVertexCount);

    // Set grid color (light grey)
    int gridVertexCountColors = gridVertexCount / 3 * 3;
    GLfloat* gridColors = malloc(gridVertexCountColors * sizeof(GLfloat));
    for (int i = 0; i < gridVertexCount / 3; ++i) {
        gridColors[i * 3 + 0] = 0.7f;
        gridColors[i * 3 + 1] = 0.7f;
        gridColors[i * 3 + 2] = 0.7f;
    }

    // Combine positions and colors
    int gridTotalVertices = gridVertexCount / 3;
    int gridTotalSize = gridTotalVertices * 6 * sizeof(GLfloat);
    GLfloat* gridData = malloc(gridTotalSize);
    for (int i = 0; i < gridTotalVertices; ++i) {
        gridData[i * 6 + 0] = gridVertices[i * 3 + 0];
        gridData[i * 6 + 1] = gridVertices[i * 3 + 1];
        gridData[i * 6 + 2] = gridVertices[i * 3 + 2];
        gridData[i * 6 + 3] = gridColors[i * 3 + 0];
        gridData[i * 6 + 4] = gridColors[i * 3 + 1];
        gridData[i * 6 + 5] = gridColors[i * 3 + 2];
    }

    GLuint gridVAO, gridVBO;
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridTotalSize, gridData, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Color attribute
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    // Clean up temporary buffers
    free(gridVertices);
    free(gridColors);
    free(gridData);

    // Initialize projection matrix
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = (float)width / (float)height;
    glm_perspective(glm_rad(45.0f), ratio, 0.1f, 100.0f, projection);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Set point size
    glPointSize(10.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // View matrix
        mat4 view;
        glm_lookat(cameraPos, (vec3){cameraPos[0] + cameraFront[0], cameraPos[1] + cameraFront[1], cameraPos[2] + cameraFront[2]}, cameraUp, view);

        // Use shader program
        glUseProgram(shaderProgram);

        // Set projection and view matrices
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const GLfloat*)projection);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view);

        // Draw axes
        mat4 model;
        glm_mat4_identity(model);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)model);

        glBindVertexArray(axesVAO);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);

        // Draw grid
        glBindVertexArray(gridVAO);
        glDrawArrays(GL_LINES, 0, gridVertexCount / 3);
        glBindVertexArray(0);

        // Draw dot
        glm_mat4_identity(model);
        glm_translate(model, (vec3){dotX, dotY, dotZ});
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)model);

        glBindVertexArray(dotVAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &dotVAO);
    glDeleteBuffers(1, &dotVBO);
    glDeleteVertexArrays(1, &axesVAO);
    glDeleteBuffers(1, &axesVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
