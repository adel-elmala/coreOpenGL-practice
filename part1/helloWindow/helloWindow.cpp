#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "shader.h"

//  --------------- globals Start here --------------- 
//  --------------- globals Start here --------------- 

float red_channel = (double)random() / RAND_MAX;
float green_channel = (double)random() / RAND_MAX;
float blue_channel = (double)random() / RAND_MAX;
double color_step = 1.0 / 256.0;

unsigned int triangle1_VAO;
unsigned int triangle1_VBO;

unsigned int triangle2_VAO;
unsigned int triangle2_VBO;

unsigned int rectangle_VBO;
unsigned int rectangle_VAO;
unsigned int rectangle_EBO;

//  --------------- globals Ends here --------------- 
//  --------------- globals Ends here --------------- 

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // std::cout << "resized!\n";
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        red_channel = (red_channel + color_step);
        green_channel = (green_channel + color_step);
        blue_channel = (blue_channel + color_step);
    }
}
void initTriangleGpuMem(void)
{
    // triangle 3d-pos in NDC
    // float verticies[] = {-0.5f, -0.5f, 0.0f,
    //                      0.5f, -0.5f, 0.0f,
    //                      0.0f, 0.5f, 0.0f};
    float verticies1[] = {
        // position        // color
        -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.7f,
        0.0f, 0.0f, 0.0f, 0.1f, 1.0f, 0.9f,
        0.0f, 1.0f, 0.0f, 0.8f, 0.6f, 1.0f // first triangle
    };

    float verticies2[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f};
    // first trinagle
    glGenVertexArrays(1, &triangle1_VAO);
    glBindVertexArray(triangle1_VAO);

    glGenBuffers(1, &triangle1_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangle1_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies1), verticies1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // second trinagle
    glGenVertexArrays(1, &triangle2_VAO);
    glBindVertexArray(triangle2_VAO);

    glGenBuffers(1, &triangle2_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangle2_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies2), verticies2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initRectangleGPUMem(void)
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left};
    };
    unsigned int indicies[6] = {0, 1, 3,
                                1, 2, 3};
    glGenVertexArrays(1, &rectangle_VAO);
    glBindVertexArray(rectangle_VAO);

    glGenBuffers(1, &rectangle_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void *)vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &rectangle_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, (void *)indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
int main(int argc, char *argv[])
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    initTriangleGpuMem();
    // initRectangleGPUMem();
    shader triangle1Shader;
    triangle1Shader.addVertexShader("./shaders/vShader.vs");
    triangle1Shader.addFragmentShader("./shaders/fShader.fs");
    triangle1Shader.createProgram();

    shader triangle2Shader;
    triangle2Shader.addVertexShader("./shaders/vShader2.vs");
    triangle2Shader.addFragmentShader("./shaders/fShader2.fs");
    triangle2Shader.createProgram();

    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        double time = glfwGetTime();
        float greenValue = (float)((sin(time) / 2.0) + 0.5);
        
        glClearColor(greenValue, green_channel, blue_channel, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // -------- render logic start -------- 
        triangle1Shader.useProgram();
        glBindVertexArray(triangle1_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        triangle2Shader.useProgram();
        triangle2Shader.setUniform4f("vertexColor", 0.2, greenValue, greenValue, 0.2);

        glBindVertexArray(triangle2_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(rectangle_VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // -------- render logic ends -------- 
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &triangle1_VAO);
    glDeleteBuffers(1, &triangle1_VBO);

    // queryInfo();
    // glDeleteVertexArrays(1, &rectangle_VAO);
    // glDeleteBuffers(1, &rectangle_VBO);
    // glDeleteBuffers(1, &rectangle_EBO);
    glfwTerminate();
    return 0;
}