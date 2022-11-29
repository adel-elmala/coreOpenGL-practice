#include <iostream>
#include <cmath>
// #include <cstdio>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "shader.h"

// ============  Declarations  ============

GLFWwindow *initWindow();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void initTriangleData();

void initCube(void);
void initTexture(const char *);
// ============  Declarations  ============

// ============  Globals  ============
unsigned int Triangle_VBO;
unsigned int Triangle_EBO;
unsigned int Triangle_VAO;

unsigned int cube_VBO;
unsigned int cube_EBO;
unsigned int cube_VAO;
// ============  Globals  ============

int main(int argc, const char *argv[])
{

    GLFWwindow *window = initWindow();

    glViewport(0, 0, 800, 800);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // initTriangleData();
    initCube();

    glActiveTexture(GL_TEXTURE0);
    initTexture("./img0.jpg");

    glActiveTexture(GL_TEXTURE1);
    initTexture("./img1.jpg");
    Shader shaderProg("./vShader.vs", "./fShader.fs");
    shaderProg.use();
    int transLoc = glGetUniformLocation(shaderProg.shaderID, "trans");
    int modelLoc = glGetUniformLocation(shaderProg.shaderID, "model");
    int viewLoc = glGetUniformLocation(shaderProg.shaderID, "view");
    int projLoc = glGetUniformLocation(shaderProg.shaderID, "projection");

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
    view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    shaderProg.setInt("tex", 0);
    shaderProg.setInt("tex2", 1);
    glClearColor(0.3, 0.3, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f,0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 transform(1.0f);
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(transform));

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        double green = (sin(time) / 2.0) + 0.5;
        unsigned int vColorLocation = glGetUniformLocation(shaderProg.shaderID, "vColor");
        shaderProg.use();
        glUniform4f(vColorLocation, 0.0f, green, 0.0f, 1.0f);
        glBindVertexArray(cube_VAO);

        for(int i = 0 ; i < 10 ; ++i)
        {
            glm::mat4 model(1.0f);

            model = glm::translate(model,cubePositions[i]);
            model = glm::rotate(model,(float)glfwGetTime()+i,glm::vec3(1.0f,1.0f,1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
    unsigned int texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(1);
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

void initCube(void)
{
    float vertices[] = {
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

    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);
    // buffer v-data
    glGenBuffers(1, &cube_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // config v-Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
    glEnableVertexAttribArray(0);
    // config v-Tex
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
