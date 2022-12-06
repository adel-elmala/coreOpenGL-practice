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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.h"

// ============  Declarations  ============

GLFWwindow *initWindow();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void initTriangleData();

void initCube(void);
void initTexture(const char *);
void mouse_callback(GLFWwindow *window, double x, double y);

void renderLightCube(Shader);
// ============  Declarations  ============

// ============  Globals  ============
unsigned int cube_VBO;
unsigned int cube_EBO;
unsigned int cube_VAO;

unsigned int lightVAO;

glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;

glm::vec3 lightPos(1.0f, 1.0f, -10.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
// ============  Globals  ============

int main(int argc, const char *argv[])
{

    GLFWwindow *window = initWindow();
    glViewport(0, 0, 800, 800);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    // glfwSetCursorPosCallback(window, mouse_callback);

    initCube();
    // glClearColor(0.3, 0.3, 0.2, 1.0);
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);

    // glActiveTexture(GL_TEXTURE1);
    // initTexture("./img1.jpg");

    Shader shaderProg("./vShader.vs", "./fShader.fs");
    shaderProg.use();

    shaderProg.setInt("mat.diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    initTexture("./container.jpg");
    shaderProg.setInt("mat.specular", 1);
    glActiveTexture(GL_TEXTURE1);
    initTexture("./container_specular.jpg");

    // initTexture("./container2.png");
    // initTexture("./img1.jpg");

    int modelLoc = glGetUniformLocation(shaderProg.shaderID, "model");
    int viewLoc = glGetUniformLocation(shaderProg.shaderID, "view");
    int projLoc = glGetUniformLocation(shaderProg.shaderID, "projection");
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // int lightColorLoc = glGetUniformLocation(shaderProg.shaderID, "lightColor");
    // int lightPosLoc = glGetUniformLocation(shaderProg.shaderID, "lightPos");
    int cameraPosLoc = glGetUniformLocation(shaderProg.shaderID, "cameraPos");
    // glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
    // glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

    int ambientLoc = glGetUniformLocation(shaderProg.shaderID, "mat.ambient");
    int specularLoc = glGetUniformLocation(shaderProg.shaderID, "mat.specular");
    int diffuseLoc = glGetUniformLocation(shaderProg.shaderID, "mat.diffuse");
    int shinenessLoc = glGetUniformLocation(shaderProg.shaderID, "mat.shineness");

    glUniform3f(ambientLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(diffuseLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(specularLoc, 0.5f, 0.5f, 0.5f);
    glUniform1f(shinenessLoc, 32.0f);

    int lightPosLoc = glGetUniformLocation(shaderProg.shaderID, "light.position");
    int lightAmbientLoc = glGetUniformLocation(shaderProg.shaderID, "light.ambient");
    int lightSpecularLoc = glGetUniformLocation(shaderProg.shaderID, "light.specular");
    int lightDiffuseLoc = glGetUniformLocation(shaderProg.shaderID, "light.diffuse");

    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightAmbientLoc, 0.3f, 0.3f, 0.3f);
    glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

    // shaderProg.setInt("tex2", 1);

    Shader lightShader("./vLightShader.vs", "./fLightShader.fs");
    renderLightCube(lightShader);
    int lightViewLoc = glGetUniformLocation(lightShader.shaderID, "view");

    float currentTime = 0.0f;
    float lastTime = 0.0f;
    // render loop
    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        processInput(window);

        shaderProg.use();
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

        glBindVertexArray(cube_VAO);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        lightShader.use();
        glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            bool show_another_window;
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //  Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

GLFWwindow *initWindow()
{
    // initialize GLFW
    glfwInit();
    const char *glsl_version = "#version 330";

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
    glfwSwapInterval(1); // Enable vsync

    // GLAD init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD: Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    float cameraSpeed = 10.0 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)

        cameraPos += cameraFront * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)

        cameraPos -= cameraFront * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)

        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)

        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);
    // buffer v-data
    glGenBuffers(1, &cube_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // config v-Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(0);

    // config v-normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    // // config v-Tex
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void mouse_callback(GLFWwindow *window, double x, double y)
{
    // std::cout << "x: " << x << "y: " << y << std::endl;
    static double lastX = x;
    static double lastY = y;
    static double pitch = 0.0;
    static double yaw = -90.0;

    double xOffset = x - lastX;
    double yOffset = y - lastY;
    double sensitivety = 0.1;
    lastX = x;
    lastY = y;

    pitch += (-yOffset) * sensitivety;
    yaw += xOffset * sensitivety;

    if (pitch > 89.0)
        pitch = 89.0;
    else if (pitch < -89.0)
        pitch = -89.0;

    glm::vec3 direction;
    direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(direction);
}

void renderLightCube(Shader lightShader)
{
    // Shader lightShader("./vLightShader.vs", "./fLightShader.fs");
    lightShader.use();
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);

    // config v-pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    int modelLoc = glGetUniformLocation(lightShader.shaderID, "model");
    int projLoc = glGetUniformLocation(lightShader.shaderID, "projection");

    glm::mat4 model(1.0f);
    glm::mat4 projection(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}