#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

long getFileSize(FILE *fp);
class Shader
{
public:
    /* data */
    // unsigned int shaderID;

    unsigned int vShaderID, fShaderID, shaderID;

public:
    Shader(const char *vertexShaderPath, const char *fragShaderPath);
    ~Shader();
    void use()
    {
        glUseProgram(shaderID);
    }
    void setBool(const char *uniformName, bool value)
    {
        glUniform1i(glGetUniformLocation(shaderID, uniformName), (int)value);
    }
    void setInt(const char *uniformName, int value)
    {
        glUniform1i(glGetUniformLocation(shaderID, uniformName), value);
    }
    void setFloat(const char *uniformName, float value)
    {
        glUniform1f(glGetUniformLocation(shaderID, uniformName), value);
    }
};

Shader::Shader(const char *vertexShaderPath, const char *fragShaderPath)
{
    // open files
    FILE *vShaderFile = fopen(vertexShaderPath, "r");
    FILE *fShaderFile = fopen(fragShaderPath, "r");
    unsigned int vShaderFileSize = getFileSize(vShaderFile);
    unsigned int fShaderFileSize = getFileSize(fShaderFile);

    // read into a char array
    unsigned char *vertexShaderSource = (unsigned char *)malloc(vShaderFileSize + 1);
    unsigned char *fragShaderSource = (unsigned char *)malloc(fShaderFileSize + 1);
    fread((void *)vertexShaderSource, vShaderFileSize, 1, vShaderFile);
    fread((void *)fragShaderSource, fShaderFileSize, 1, fShaderFile);
    // add null-terminator to the string
    vertexShaderSource[vShaderFileSize] = '\0';
    fragShaderSource[fShaderFileSize] = '\0';
    // close files
    fclose(vShaderFile);
    fclose(fShaderFile);
    // make shaders
    // unsigned int vShaderID, fShaderID, shaderID;
    vShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShaderID, 1, (const char **)&vertexShaderSource, NULL);
    glCompileShader(vShaderID);
    int status;
    char logInfo[512];
    glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(vShaderID, 512, NULL, logInfo);
        fprintf(stderr, "Failed to compile vertex shader\n%s.", logInfo);
    }

    fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fShaderID, 1, (const char **)&fragShaderSource, NULL);
    glCompileShader(fShaderID);
    glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(fShaderID, 512, NULL, logInfo);
        fprintf(stderr, "Failed to compile fragment shader\n%s.", logInfo);
    }

    // ...

    shaderID = glCreateProgram();
    glAttachShader(shaderID, vShaderID);
    glAttachShader(shaderID, fShaderID);
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(shaderID, 512, NULL, logInfo);
        fprintf(stderr, "Failed to link shader program\n\n%s.", logInfo);
    }

    free(vertexShaderSource);
    free(fragShaderSource);
}

Shader::~Shader()
{
    // glDeleteShader(vShaderID);
    // glDeleteShader(fShaderID);
    // glDeleteProgram(shaderID);
}

long getFileSize(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    if (fsize < 0)
    {
        perror(NULL);
        exit(EXIT_FAILURE);
    }
    return fsize;
}
#endif