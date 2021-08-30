#include "windowIO.h"
#include "shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <math.h>

// --------- globals -------------
unsigned int rect_vao;
unsigned int rect_ebo;
unsigned int rect_vbo;
unsigned int rect_tex;
const int winWidth = 720;
const int winHeight = 500;

glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

// --------- globals -------------

// --------- forward declerations -------------
void initCubeGPUMem(void);
void initTextures(const char *imgPath, unsigned int *texID, GLenum texUnit = GL_TEXTURE0);
void processInput(GLFWwindow *window);
void cursorCallBack(GLFWwindow *window, double x, double y);

// --------- forward declerations -------------

int main(int argc, char *argv[])
{
    GLFWwindow *window = initWindow(winWidth, winHeight, "Boiler-Plate!");
    glViewport(0, 0, winWidth, winHeight);
    glEnable(GL_DEPTH_TEST);

    initCubeGPUMem();
    shader rectShader;
    rectShader.addVertexShader("./shaders/vShader.vert");
    rectShader.addFragmentShader("./shaders/fShader.frag");
    rectShader.createProgram();
    rectShader.useProgram();

    initTextures("./resources/img2.jpg", &rect_tex);
    rectShader.setUniform1i("texture", 0);

    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)winWidth) / winHeight, 0.1f, 100.f);
    // glm::mat4 view = glm::translate(identity, glm::vec3(0.0f, 0.0f, -3.0f));

    unsigned int modelLoc = glGetUniformLocation(rectShader.programID, "model");
    unsigned int viewLoc = glGetUniformLocation(rectShader.programID, "view");
    unsigned int projectionLoc = glGetUniformLocation(rectShader.programID, "projection");

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursorCallBack);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        float val = sin(currentTime);

        glClearColor(val, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(rect_vao);
        float radius = 20.0f;
        // float camX = cos((float)time) * radius;
        // float camZ = val * radius;
        glm::mat4 view = glm::lookAt(camPos,
                                     camPos + camFront,
                                     camUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        for (int i = 0; i < 10; ++i)
        {

            glm::mat4 model = glm::translate(identity, cubePositions[i]);
            model = glm::rotate(model, currentTime * glm::radians(10.f * (i + 1)), glm::vec3(1.0f, val, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void initCubeGPUMem(void)
{
    // float vertices[] = {
    //     // position      // texture coords
    //     0.5f, 0.5f, 0.0f, 2.0f, 2.0f,   // top right
    //     0.5f, -0.5f, 0.0f, 2.0f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    //     -0.5f, 0.5f, 0.0f, 0.0f, 2.0f   // top left};
    // };

    float vertices[] = {
        // position      // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    // unsigned int indicies[6] = {0, 1, 3,
    //                             1, 2, 3};

    glGenVertexArrays(1, &rect_vao);
    glBindVertexArray(rect_vao);
    // buffer vert-data
    glGenBuffers(1, &rect_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // glGenBuffers(1, &rect_ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect_ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initTextures(const char *imgPath, unsigned int *texID, GLenum texUnit)
{
    glGenTextures(1, texID);
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, *texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channles;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imgPath, &width, &height, &channles, 0);
    if (data != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void *)data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        fprintf(stderr, "Failed to load image: [%s]\n", imgPath);
    stbi_image_free(data);
}

void processInput(GLFWwindow *window)
{
    const float camSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)

        camPos += camFront * camSpeed;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camPos -= camFront * camSpeed;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camPos += glm::normalize(glm::cross(camUp, camFront)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camPos -= glm::normalize(glm::cross(camUp, camFront)) * camSpeed;
}

void cursorCallBack(GLFWwindow *window, double x, double y)
{
    static double lastX = winWidth / 2;
    static double lastY = winHeight / 2;
    static bool firstMouse = true;
    static double pitch = 0;
    static double yaw = 180;

    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    double offsetX = x - lastX;
    double offsetY = lastY - y;
    lastX = x;
    lastY = y;

    const double sensitivity = 0.1;
    offsetX *= sensitivity;
    offsetY *= sensitivity;

    yaw -= offsetX;
    pitch += offsetY;

    if (pitch > 89.0)
        pitch = 89.0;
    if (pitch < -89.0)
        pitch = -89.0;

    glm::vec3 direction;
    direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(direction);
}