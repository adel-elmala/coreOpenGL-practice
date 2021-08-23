#include "shader.h"

shader::shader(/* args */)
{
    fprintf(stdout, "Shader Constructed!\n");
}

shader::~shader()
{
    fprintf(stdout, "Shader Deconstructed!\n");
    
    // seg. faults for some reason
    // glDeleteProgram(this->programID);
}

long shader::getFileSize(const char *filePath)
{
    FILE *fd = fopen(filePath, "r");
    fseek(fd, 0, SEEK_END);
    long size = ftell(fd);
    fclose(fd);
    // fprintf(stdout, "[%s]'s size is: %ld [bytes]\n", filePath, size);

    return size;
}

bool shader::addVertexShader(const char *filePath)
{
    // open file
    FILE *fd = fopen(filePath, "r");
    long nBytes = getFileSize(filePath);
    char *vertexSource = (char *)malloc(nBytes * sizeof(char) + 1);
    // read file into memory
    fread((void *)vertexSource, 1, nBytes, fd);
    // add null-terminator
    vertexSource[nBytes] = '\0';

    this->vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->vShader, 1, &vertexSource, NULL);
    glCompileShader(this->vShader);

    // fprintf(stdout, "vertex Source:\n%s\n", vertexSource);

    free(vertexSource);
    int status;
    glGetShaderiv(this->vShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char log[512];
        glGetShaderInfoLog(this->vShader, 512, NULL, log);
        fprintf(stderr, "Vertex Shader ERROR: %s", log);
        return false;
    }
    else
        return true;
}
bool shader::addFragmentShader(const char *filePath)
{
    // open file
    FILE *fd = fopen(filePath, "r");
    long nBytes = getFileSize(filePath);
    char *fragmentSource = (char *)malloc(nBytes * sizeof(char) + 1);
    // read file into memory
    fread((void *)fragmentSource, 1, nBytes, fd);
    // add null-terminator
    fragmentSource[nBytes] = '\0';

    this->fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(this->fShader, 1, &fragmentSource, NULL);
    glCompileShader(this->fShader);
    // fprintf(stdout, "frag Source:\n%s\n", fragmentSource);

    free(fragmentSource);

    int status;
    glGetShaderiv(this->fShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char log[512];
        glGetShaderInfoLog(this->fShader, 512, NULL, log);
        fprintf(stderr, "Fragment Shader ERROR: %s", log);
        return false;
    }
    else
        return true;
}

bool shader::createProgram()
{
    this->programID = glCreateProgram();
    glAttachShader(this->programID, this->vShader);
    glAttachShader(this->programID, this->fShader);
    glLinkProgram(this->programID);

    glDeleteShader(this->vShader);
    glDeleteShader(this->fShader);

    int status;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &status);
    if (!status)
    {
        char log[512];
        glGetProgramInfoLog(this->programID, 512, NULL, log);
        fprintf(stderr, "Shader Program ERROR: %s", log);
        return false;
    }
    else
    {
        return true;
    }
}

void shader::useProgram()
{
    glUseProgram(this->programID);
}

void shader::setUniform4f(const char *name, float v0, float v1, float v2, float v3)
{
    int uniformLocation = glGetUniformLocation(this->programID, name);
    glUniform4f(uniformLocation, v0,v1,v2,v3);
}
