#pragma once

#include <string>
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

    // use and unuse program
    void use();
    void unuse();
private:
    int _numAttributes;

    GLuint _programId;
    GLuint _vertexShaderId;
    GLuint _fragmentShaderId;

    // compile a single shader
    void _compileShader(const std::string& filePath, GLuint id);
};

