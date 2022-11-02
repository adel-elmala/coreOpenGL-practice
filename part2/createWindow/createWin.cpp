#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>
// #include <cstdio>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLFWwindow *initWindow();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void initTriangleData();

void initTexture(const char *);
// Globals

unsigned int Triangle_VBO;
unsigned int Triangle_EBO;
unsigned int Triangle_VAO;

int main(int argc, const char *argv[])
{

    GLFWwindow *window = initWindow();

    glViewport(0, 0, 800, 800);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    initTriangleData();
    initTexture("./texImg.jpg");
    Shader shaderProg("./vShader.vs", "./fShader.fs");

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.3, 0.3, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        double time = glfwGetTime();
        double green = (sin(time) / 2.0) + 0.5;
        unsigned int vColorLocation = glGetUniformLocation(shaderProg.shaderID, "vColor");
        shaderProg.use();
        glUniform4f(vColorLocation, 0.0f, green, 0.0f, 1.0f);
        glBindVertexArray(Triangle_VAO);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow *initWindow()
{
    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "create Window!", NULL, NULL);
    if (!window)
    {
        std::cerr << "GLFW: Failed to make a window object!" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    // GLAD init

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD: Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void initTriangleData()
{
    // float verticies[] = {1.0f, 0.0f, 0.0f, // Triangle 1 (top right)
    //                      0.0f, 1.0f, 0.0f,
    //                      0.0f, 0.0f, 0.0f};

    float verticies[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f}; // top left

    unsigned int indices[] = {0, 1, 3,  // first triangle
                              1, 2, 3}; // second triangle

    glGenVertexArrays(1, &Triangle_VAO);
    glBindVertexArray(Triangle_VAO);
    // buffer v-data
    glGenBuffers(1, &Triangle_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Triangle_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    // config v-Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(0);
    // config v-Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // config v-Tex
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);
    // buffer idx-data
    glGenBuffers(1, &Triangle_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Triangle_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void initTexture(const char *pathName)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nChannels;
    unsigned char *data = stbi_load(pathName, &width, &height, &nChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "STB: Failed to load image: %s\n", pathName);
    }
    stbi_image_free(data);
}