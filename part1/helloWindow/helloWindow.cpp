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
unsigned int triangle1_VAO;
unsigned int triangle1_VBO;
unsigned int shaderprog1;

unsigned int triangle2_VAO;
unsigned int triangle2_VBO;
unsigned int shaderprog2;

unsigned int rectangle_VBO;
unsigned int rectangle_VAO;
unsigned int rectangle_EBO;

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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3*sizeof(float)));
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

void initTriangleShaders()
{
    const char *const vShaderSource = "#version 330 core\n\
        layout(location = 0) in vec3 aPos;\n\
        layout(location = 1) in vec3 aColor;\n\
        out vec3 pixelColor;\n\
        void main()\n\
        {\
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
            pixelColor = aColor;\n\
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
        in vec3 pixelColor;\n\
        out vec4 FragColor;\n\
        void main()\n\
        {\n\
            // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
            FragColor = vec4(pixelColor, 1.0f);\n\
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

    shaderprog1 = glCreateProgram();
    glAttachShader(shaderprog1, vShaderID);
    glAttachShader(shaderprog1, fShaderID);
    glLinkProgram(shaderprog1);

    int pStatus;
    char pInfoLog[512];
    glGetProgramiv(shaderprog1, GL_LINK_STATUS, &pStatus);
    if (!pStatus)
    {
        glGetProgramInfoLog(shaderprog1, 512, NULL, pInfoLog);
        fprintf(stderr, "Shader program Failed to link.\n%s\n", pInfoLog);
    }
    // glUseProgram(shaderprog1);

    glDeleteShader(vShaderID);
    glDeleteShader(fShaderID);
}

void initTriangle2Shaders()
{
    const char *vShaderSource = "#version 330 core\n\
        layout(location = 0) in vec3 aPos;\n\
        // out vec4 vertexColor;\n\
        void main()\n\
        {\
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
            // vertexColor = vec4(0.9,0.2,0.2,1.0);\n\
        }\0";

    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSource, NULL);
    glCompileShader(vShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);
    }

    const char *fShaderSource = "#version 330 core\n\
        uniform vec4 vertexColor;\n\
        out vec4 FragColor;\n\
        void main()\n\
        {\n\
            // FragColor = vec4(0.1f, 0.8f, 0.1f, 1.0f);\n\
            FragColor = vertexColor;\n\
        }\0";

    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSource, NULL);
    glCompileShader(fShader);

    int success2;
    char infoLog2[512];
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success2);
    if (!success2)
    {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog2);
        fprintf(stderr, "Fragment Shader Error: %s\n", infoLog2);
    }

    shaderprog2 = glCreateProgram();
    glAttachShader(shaderprog2, vShader);
    glAttachShader(shaderprog2, fShader);
    glLinkProgram(shaderprog2);

    int pStatus;
    char pInfoLog[512];
    glGetProgramiv(shaderprog2, GL_LINK_STATUS, &pStatus);
    if (!pStatus)
    {
        glGetProgramInfoLog(shaderprog2, 512, NULL, pInfoLog);
        fprintf(stderr, "Shader program Failed to link.\n%s\n", pInfoLog);
    }
    // glUseProgram(shaderprog2);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void queryInfo(void)
{
    int attribLen;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribLen);
    fprintf(stdout, "Maximum nr of vertex attributes supported: %d\n", attribLen);
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
    initTriangleShaders();
    initTriangle2Shaders();

    int vertexColorLocation = glGetUniformLocation(shaderprog2, "vertexColor");

    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        double time = glfwGetTime();
        float greenValue = (float)((sin(time) / 2.0) + 0.5);
        processInput(window);
        glClearColor(greenValue, green_channel, blue_channel, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render loop
        glUseProgram(shaderprog1);
        glBindVertexArray(triangle1_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glUseProgram(shaderprog2);
        glUniform4f(vertexColorLocation, 0.2, greenValue, 0.2, 0.2);
        glBindVertexArray(triangle2_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(rectangle_VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &triangle1_VAO);
    glDeleteBuffers(1, &triangle1_VBO);

    queryInfo();
    // glDeleteVertexArrays(1, &rectangle_VAO);
    // glDeleteBuffers(1, &rectangle_VBO);
    // glDeleteBuffers(1, &rectangle_EBO);
    glfwTerminate();
    return 0;
}