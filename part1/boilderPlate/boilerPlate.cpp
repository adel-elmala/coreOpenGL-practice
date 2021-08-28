#include "windowIO.h"

unsigned int rect_vao;
unsigned int rect_vbo;

int main(int argc, char *argv[])
{
    GLFWwindow *window = initWindow(720, 500, "Boiler-Plate!");
    glViewport(0, 0, 720, 500);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
