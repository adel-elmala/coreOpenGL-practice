#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
// int #include <cstdio>

GLFWwindow *initWindow();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void initTriangleData();

void initVShader();
void initFShader();
void createShaderProg();

// Globals

unsigned int Triangle_VBO;
unsigned int Triangle_EBO;
unsigned int Triangle_VAO;

unsigned int vShaderID;
unsigned int fShaderID;
unsigned int shaderProg;

int main(int argc, const char *argv[])
{

    GLFWwindow *window = initWindow();

    glViewport(0, 0, 800, 800);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    initTriangleData();
    initVShader();
    initFShader();
    createShaderProg();

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2, 0.3, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProg);
        glBindVertexArray(Triangle_VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

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
    // float verticies[] = {-0.5f, -0.5f, 0.0f,
    //                      0.5f, -0.5f, 0.0f,
    //                      0.0f, 0.5f, 0.0f};

    float verticies[] = {0.5f, 0.5f, 0.0f,   // top right
                         0.5f, -0.5f, 0.0f,  // bottom right
                         -0.5f, -0.5f, 0.0f, // bottom left
                         -0.5f, 0.5f, 0.0f}; // top left

    unsigned int indices[] = {0, 1, 3,  // first triangle
                              1, 2, 3}; // second triangle

    glGenBuffers(1, &Triangle_VBO);
    glGenBuffers(1, &Triangle_EBO);

    glGenVertexArrays(1, &Triangle_VAO);
    glBindVertexArray(Triangle_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Triangle_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, Triangle_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);
}

void initVShader()
{
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 vpos;\n"
                                     "void main(){\n"
                                     " gl_Position = vec4(vpos,1.0f);\n"
                                     "}";

    vShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShaderID, 1, &vertexShaderSource, NULL);
    glCompileShader(vShaderID);
    int status;
    char logInfo[512];
    glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(vShaderID, 512, NULL, logInfo);
        std::cerr << "Failed to compile vertex shader\n " << logInfo << std::endl;
    }
}

void initFShader()
{
    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 color;\n"
                                       "void main(){\n"
                                       " color = vec4(0.2f,0.6f,0.7f,1.0f);\n"
                                       "}";

    fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderID, 1, &fragmentShaderSource, NULL);
    glCompileShader(fShaderID);
    int status;
    char logInfo[512];
    glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(fShaderID, 512, NULL, logInfo);
        std::cerr << "Failed to compile fragment shader\n " << logInfo << std::endl;
    }
}

void createShaderProg()
{
    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vShaderID);
    glAttachShader(shaderProg, fShaderID);
    glLinkProgram(shaderProg);
    int status;
    char logInfo[512];
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(shaderProg, 512, NULL, logInfo);
        std::cerr << "Failed to link shader program\n " << logInfo << std::endl;
    }
}