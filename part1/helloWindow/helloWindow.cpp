#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

//  globals
float red_channel = (double)random() / RAND_MAX;
float green_channel = (double)random() / RAND_MAX;
float blue_channel = (double)random() / RAND_MAX;

double color_step = 1.0 / 256.0;
unsigned int triangle_VAO ;
unsigned int triangle_VBO ;

unsigned int rectangle_VBO ;
unsigned int rectangle_VAO ;
unsigned int rectangle_EBO ;

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
    float verticies[] = {-0.5f, -0.5f, 0.0f,
                         0.5f, -0.5f, 0.0f,
                         0.0f, 0.5f, 0.0f};

    // unsigned int vao = 0;
    glGenVertexArrays(1, &triangle_VAO);
    glBindVertexArray(triangle_VAO);

    // unsigned int vbo = 0;
    glGenBuffers(1, &triangle_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void initTriangleShaders()
{
    const char *const vShaderSource = "#version 330 core\n\
        layout(location = 0) in vec3 aPos;\n\
        void main()\n\
        {\
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\
        }\0";

    unsigned int vShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShaderID, 1, &vShaderSource, NULL);
    glCompileShader(vShaderID);

    int status;
    char infoLog[512];
    glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(vShaderID, 512, NULL, infoLog);
        fprintf(stderr, "Vertex Shader Failed to compile.\n%s\n", infoLog);
    }

    const char *const fShaderSource = "#version 330 core\n\
        out vec4 FragColor;\n\
        void main()\n\
        {\n\
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
        }\0";
    unsigned int fShaderID = 0;
    fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderID, 1, &fShaderSource, NULL);
    glCompileShader(fShaderID);

    int fStatus;
    char fInfoLog[512];
    glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &fStatus);
    if (!fStatus)
    {
        glGetShaderInfoLog(fShaderID, 512, NULL, fInfoLog);
        fprintf(stderr, "Fragment Shader Failed to compile.\n%s\n", fInfoLog);
    }

    unsigned int shaderProgram = 0;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShaderID);
    glAttachShader(shaderProgram, fShaderID);
    glLinkProgram(shaderProgram);

    int pStatus;
    char pInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &pStatus);
    if (!pStatus)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, pInfoLog);
        fprintf(stderr, "Shader program Failed to link.\n%s\n", pInfoLog);
    }
    glUseProgram(shaderProgram);

    glDeleteShader(vShaderID);
    glDeleteShader(fShaderID);
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
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
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
    initTriangleShaders();
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(red_channel, green_channel, blue_channel, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // render loop
        glBindVertexArray(triangle_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(rectangle_VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&triangle_VAO);
    glDeleteBuffers(1,&triangle_VBO);
    glDeleteVertexArrays(1,&rectangle_VAO);
    glDeleteBuffers(1,&rectangle_VBO);
    glDeleteBuffers(1,&rectangle_EBO);
    glfwTerminate();
    return 0;
}