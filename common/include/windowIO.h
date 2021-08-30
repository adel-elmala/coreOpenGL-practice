#ifndef WINDOWIO_H 
#define WINDOWIO_H 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>


GLFWwindow *initWindow(int width, int height, const char *title);
// void processInput(GLFWwindow *window);
void resizeCallBack(GLFWwindow *window, int width, int height);

#endif