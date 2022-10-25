#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
// int #include <cstdio>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int main(int argc, const char *argv[])
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
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD init

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD: Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 800);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2,0.3,0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
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