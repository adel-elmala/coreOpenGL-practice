#ifndef SHADER_H
#define SHADER_H

#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class shader
{
private:
    /* data */
    unsigned int programID;
    unsigned int vShader;
    unsigned int fShader;

public:
    shader();
    ~shader();
    // returns the No. of bytes for a file
    long getFileSize(const char *filePath);
    bool addVertexShader(const char *filePath);
    bool addFragmentShader(const char *filePath);
    bool createProgram();
    void useProgram();
    // uniform shader variable setters
    void setUniform4f(const char *name, float v0, float v1, float v2, float v3);
    void setUniform1i(const char *name, int value);

};

#endif // SHADER_H
