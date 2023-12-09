#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {}

ShaderProgram::~ShaderProgram() {}

void 
ShaderProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {

    m_programID = glCreateProgram();

    m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

    if (m_vertexShaderId == 0) {
        std::printf("glCreateShader() on %s error.\n", vertexShaderFilePath.c_str());
    }

    m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    if (m_fragmentShaderId == 0) {
        std::printf("glCreateShader() on %s error.\n", fragmentShaderFilePath.c_str());
    }

    compileShader(vertexShaderFilePath, m_vertexShaderId);
    compileShader(fragmentShaderFilePath, m_fragmentShaderId);
}

void 
ShaderProgram::compileShader(const std::string& filePath, GLuint id) {
    std::ifstream shaderFile(filePath);

    if (shaderFile.fail()) {
        perror(filePath.c_str());
        std::printf("Failed to open %s\n", filePath.c_str());
    }

    std::string fileContent = "";
    std::string line;

    while (std::getline(shaderFile, line)) {
        fileContent += line + "\n";
    }

    shaderFile.close();

    const char* contentsPointer = fileContent.c_str();

    glShaderSource(id, 1, &contentsPointer, nullptr);

    glCompileShader(id);

    GLint isCompiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

        std::printf("%s", &(errorLog[0]));
        std::printf("Shader %s failed to compile\n", filePath.c_str());

        glDeleteShader(id);
    }
}

void 
ShaderProgram::linkShaders() {

    glAttachShader(m_programID, m_vertexShaderId);
    glAttachShader(m_programID, m_fragmentShaderId);

    glLinkProgram(m_programID);

    GLint isLinked = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

        glDeleteProgram(m_programID);
        
        glDeleteShader(m_vertexShaderId);
        glDeleteShader(m_fragmentShaderId);

        std::printf("%s", &(errorLog[0]));
        std::printf("Shaders failed to link.\n");
        return;
    }

    glDetachShader(m_programID, m_vertexShaderId);
    glDetachShader(m_programID, m_fragmentShaderId);

    glDeleteShader(m_vertexShaderId);
    glDeleteShader(m_fragmentShaderId);
}

void 
ShaderProgram::addAttribute(const std::string& attributeName) {
    glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
}

GLint 
ShaderProgram::getUniformLocation(const std::string& uniformName) {
    GLint location = glGetUniformLocation(m_programID, uniformName.c_str());

    if (location == GL_INVALID_INDEX) {
        std::printf("Uniform %s not found in the shader.\n", uniformName.c_str());
    }
    return location;
}

void 
ShaderProgram::use() {
    glUseProgram(m_programID);
}

void 
ShaderProgram::unuse() {
    glUseProgram(0);
}