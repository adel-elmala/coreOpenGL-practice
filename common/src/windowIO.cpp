#include "windowIO.h"


GLFWwindow *initWindow(int width, int height, const char *title)
{
    glfwInit();
    // openGL v3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "[GLFW] Failed to create a window!\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "[GLAD] Failed to initialize GLAD\n");
        glfwTerminate();
        return NULL;
    }
    glfwSetFramebufferSizeCallback(window, resizeCallBack);

    return window;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void resizeCallBack(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}