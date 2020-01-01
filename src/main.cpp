#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include "Shader.h"

int zoom = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if((yoffset > 0 && zoom < 10) || (yoffset < 0 && zoom > 0))
        zoom += yoffset;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
/* // Test code for boundaries
if (tempTopRight.x + offX > 1) {
    offX -= tempTopRight.x + offX - 1;
}
if (tempTopRight.y + offY > 1) {
    offY -= tempTopRight.y + offY - 1;
}
if (tempBottomLeft.x + offX < -1) {
    offX -= tempTopRight.x + offX + 1;
}
if (tempBottomLeft.y + offY < -1) {
    offY -= tempTopRight.y + offY + 1;
}
*/
void checkBoundaries(glm::vec4 &tempTopRight, glm::vec4 &tempBottomLeft, float offX, float offY, glm::mat4 &trans) {
    // If not touching any boundary, add offsets
    if ((tempTopRight.x > 1 && tempTopRight.y > 1) && (tempBottomLeft.x < -1 && tempBottomLeft.y < -1)) {
        trans = glm::translate(trans, glm::vec3(offX, offY, 0.0f));
    }
    else { // If-statements for the corners to enable side-panning at boundaries
        if (tempTopRight.x <= 1) {
            if (offX > 0)
                trans = glm::translate(trans, glm::vec3(offX, 0.0f, 0.0f));
            if (tempTopRight.y > 1 && tempBottomLeft.y < -1)
                trans = glm::translate(trans, glm::vec3(0.0f, offY, 0.0f));
        }
        if (tempTopRight.y <= 1) {
            if (offY > 0)
                trans = glm::translate(trans, glm::vec3(0.0f, offY, 0.0f));
            if (tempTopRight.x > 1 && tempBottomLeft.x < -1)
                trans = glm::translate(trans, glm::vec3(offX, 0.0f, 0.0f));
        }
        if (tempBottomLeft.x >= -1) {
            if (offX < 0)
                trans = glm::translate(trans, glm::vec3(offX, 0.0f, 0.0f));
            if (tempBottomLeft.y < -1 && tempTopRight.y > 1)
                trans = glm::translate(trans, glm::vec3(0.0f, offY, 0.0f));
        }
        if (tempBottomLeft.y >= -1) {
            if (offY < 0)
                trans = glm::translate(trans, glm::vec3(0.0f, offY, 0.0f));
            if (tempBottomLeft.x < -1 && tempTopRight.x > 1)
                trans = glm::translate(trans, glm::vec3(offX, 0.0f, 0.0f));
        }
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Triangles with differently colored vertexes
    float triangleVertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    // Triangles without color values
    /*float triangleVertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
    };*/

    float rectangleVertices[] = {
     1.0f,  1.0f, 0.0f,  // top right
     1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f   // top left 
    };
    unsigned int rectangleIndices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    int nrOfDots = 1000;
    float* dots;
    dots = new float[1000];
    //float dots[100000];

    // Around 100000 randomly placed dots
    /*
    for (int i = 0; i < nrOFDots - 2; i = i + 3) {
        dots[i] = (float)rand() / RAND_MAX;
        dots[i + 1] = (float)rand() / RAND_MAX;
        dots[i + 2] = (rand() % 3) + 1; // 3 colors
        if (rand() % 2 == 0) {
            dots[i] = -dots[i];
        }
        if (rand() % 2 == 0) {
            dots[i+1] = -dots[i+1];
        }
    }
    */

    // Dots of each set placed in certain areas to test fragnent shader color management
    for (int i = 0; i < (nrOfDots - 2); i = i + 3) {
        if (i < (nrOfDots / 3)) {
            dots[i] = -(float)rand() / RAND_MAX;
            dots[i + 1] = -(float)rand() / RAND_MAX;
            dots[i + 2] = 1; // First set is placed at lower left quadrant
        }
        else if (i < (2 * nrOfDots / 3)) {
            dots[i] = (float)rand() / RAND_MAX;
            dots[i + 1] = (float)rand() / RAND_MAX;
            dots[i + 2] = 2; // Second set is placed at top right quadrant
        }
        else {
            dots[i] = (float)rand() / RAND_MAX;
            dots[i + 1] = -(float)rand() / RAND_MAX;
            dots[i + 2] = 3; // Second set is placed at bottom right quadrant
        }
    }

    // VAO containing full-canvas rectangle
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);
    glBindVertexArray(0);


    // VAO2 containing dots
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    // Triangle on the canvas
    /*
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    */

    // Dots on the canvas
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 1000, dots, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    /*
     GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*1000, dots, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    */
   
    // Shader Storage Object to iterate all vertexes when deciding color in fragment shader
    
    float data[] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), data, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    
    

    Shader ourShader("shader_v.vs", "shader_f.fs");
   // int vertexColorLocation;
    int height, width;
    double xPos, yPos;
    double prevX, prevY;
    float offX = 0.0f, offY = 0.0f;
    bool firstClick = true;
    int currentZoom = 0;
    glm::mat4 trans = glm::mat4(1.0f);
    glm::vec4 topRightCorner = glm::vec4(rectangleVertices[0], rectangleVertices[1], 0.0, 1.0);
    glm::vec4 bottomLeftCorner = glm::vec4(rectangleVertices[6], rectangleVertices[7], 0.0, 1.0);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwGetWindowSize(window, &width, &height);

        ourShader.use();
        ourShader.setVec3("color", 1.0f, 1.0f, 1.0f);
        ourShader.setMat4("trans", trans);

        glfwGetCursorPos(window, &xPos, &yPos); // Cannot be before shader calls for some reason

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        memcpy(p, &data, sizeof(data));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ourShader.setVec3("color", 0.0f, 0.0f, 0.0f);
        glBindVertexArray(VAO2);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_POINTS, 0, 900);
        // Pan view here if mouse coordinates change
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (firstClick) {
                glfwGetCursorPos(window, &prevX, &prevY);
                firstClick = false;
            }
            else {
                if ((xPos != prevX) || (yPos != prevY)) {
                    if (xPos != prevX)
                        offX = (xPos - prevX) / width;
                    else offX = 0.0f;
                    if (yPos != prevY)
                        offY = -(yPos - prevY) / height;
                    else offY = 0.0f;
                    glm::vec4 tempTopRight = trans * topRightCorner;
                    glm::vec4 tempBottomLeft = trans * bottomLeftCorner;
                    checkBoundaries(tempTopRight, tempBottomLeft, offX, offY, trans);
                   
                }
                glfwGetCursorPos(window, &prevX, &prevY);
            }
        }
        else {
            if (!firstClick) {
                firstClick = true;
            }
        }
        if (currentZoom > zoom) { // Zooming out
            trans = glm::scale(trans, glm::vec3(10.0f / 11.0f, 10.0f / 11.0f, 0.0f));
        }
        else if(currentZoom < zoom){ // Zooming in
            trans = glm::translate(trans, glm::vec3((xPos - width / 2) / (width / 2), -(yPos - height / 2) / (height / 2), 0.0f));
            trans = glm::scale(trans, glm::vec3(1.1f, 1.1f, 0.0f));
            trans = glm::translate(trans, glm::vec3(-(xPos - width / 2) / (width / 2), (yPos - height / 2) / (height / 2), 0.0f));
        }
        else if (zoom == 0) {
            trans = glm::mat4(1.0f); // Center view if fully zoomed out
        }
        currentZoom = zoom;
        
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}