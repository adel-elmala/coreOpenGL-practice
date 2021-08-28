#include "windowIO.h"
#include "shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <math.h>

// --------- globals -------------
unsigned int rect_vao;
unsigned int rect_ebo;
unsigned int rect_vbo;
unsigned int rect_tex;
const int winWidth = 720;
const int winHeight = 500;
// --------- globals -------------

// --------- forward declerations -------------
void initCubeGPUMem(void);
void initTextures(const char *imgPath, unsigned int *texID, GLenum texUnit = GL_TEXTURE0);

// --------- forward declerations -------------

int main(int argc, char *argv[])
{
    GLFWwindow *window = initWindow(winWidth, winHeight, "Boiler-Plate!");
    glViewport(0, 0, winWidth, winHeight);
    glEnable(GL_DEPTH_TEST);

    initCubeGPUMem();
    shader rectShader;
    rectShader.addVertexShader("./shaders/vShader.vert");
    rectShader.addFragmentShader("./shaders/fShader.frag");
    rectShader.createProgram();
    rectShader.useProgram();

    initTextures("./resources/img1.jpg", &rect_tex);
    rectShader.setUniform1i("texture", 0);

    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 view = glm::translate(identity, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)winWidth) / winHeight, 0.1f, 100.f);

    unsigned int modelLoc = glGetUniformLocation(rectShader.programID, "model");
    unsigned int viewLoc = glGetUniformLocation(rectShader.programID, "view");
    unsigned int projectionLoc = glGetUniformLocation(rectShader.programID, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        double time = glfwGetTime();
        float val = sin((float)time);

        glm::mat4 model = glm::rotate(identity, (float)time * glm::radians(-50.f), glm::vec3(1.0f, val, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glClearColor(val, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(rect_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void initCubeGPUMem(void)
{
    // float vertices[] = {
    //     // position      // texture coords
    //     0.5f, 0.5f, 0.0f, 2.0f, 2.0f,   // top right
    //     0.5f, -0.5f, 0.0f, 2.0f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    //     -0.5f, 0.5f, 0.0f, 0.0f, 2.0f   // top left};
    // };

    float vertices[] = {
        // position      // texture coords
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

    // unsigned int indicies[6] = {0, 1, 3,
    //                             1, 2, 3};

    glGenVertexArrays(1, &rect_vao);
    glBindVertexArray(rect_vao);
    // buffer vert-data
    glGenBuffers(1, &rect_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // glGenBuffers(1, &rect_ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect_ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initTextures(const char *imgPath, unsigned int *texID, GLenum texUnit)
{
    glGenTextures(1, texID);
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, *texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channles;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imgPath, &width, &height, &channles, 0);
    if (data != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void *)data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        fprintf(stderr, "Failed to load image: [%s]\n", imgPath);
    stbi_image_free(data);
}
