#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    // compile all the shader
    void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

    // link the shaders
    void linkShaders();

    // add a vertext shader attribute
    void addAttribute(const std::string& attributeName);

    GLint getUniformLocation(const std::string& uniformName);

    // use and unuse program
    void use();
    void unuse();
private:
    int m_numAttributes;

    GLuint m_programID;
    GLuint m_vertexShaderId;
    GLuint m_fragmentShaderId;

    // compile a single shader
    void compileShader(const std::string& filePath, GLuint id);
};

