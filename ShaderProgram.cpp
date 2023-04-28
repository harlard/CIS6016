#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram() : programId(0) {}

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) : programId(0) {
    load(vertexPath, fragmentPath);
}

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath) : programId(0) {
    load(vertexPath, geometryPath, fragmentPath);
}

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& tessControlPath, const std::string& tessEvalPath, const std::string& fragmentPath) : programId(0) {
    load(vertexPath, tessControlPath, tessEvalPath, fragmentPath);
}

ShaderProgram::~ShaderProgram() {
    if (programId != 0) {
        glDeleteProgram(programId);
    }
}

void ShaderProgram::load(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    try {
        std::ifstream vertexFile(vertexPath);
        std::ifstream fragmentFile(fragmentPath);
        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error: Failed to read shader file" << std::endl;
        return;
    }

    const char* vertexCodePtr = vertexCode.c_str();
    const char* fragmentCodePtr = fragmentCode.c_str();

    unsigned int vertexShaderId = compileShader(GL_VERTEX_SHADER, vertexCodePtr);
    unsigned int fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragmentCodePtr);

    linkProgram(vertexShaderId, 0, 0, 0, fragmentShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

void ShaderProgram::load(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath) {
    // Load and compile the vertex shader
    std::string vertexCode = readShaderFile(vertexPath);
    unsigned int vertexShaderId = compileShader(GL_VERTEX_SHADER, vertexCode);

    // Load and compile the geometry shader, if a geometry shader file path is provided
    unsigned int geometryShaderId = 0;
    if (!geometryPath.empty()) {
        std::string geometryCode = readShaderFile(geometryPath);
        geometryShaderId = compileShader(GL_GEOMETRY_SHADER, geometryCode);
    }

    // Load and compile the fragment shader
    std::string fragmentCode = readShaderFile(fragmentPath);
    unsigned int fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    // Link the shader program
    linkProgram(vertexShaderId, geometryShaderId, 0, 0, fragmentShaderId);
}

void ShaderProgram::load(const std::string& vertexPath, const std::string& tessControlPath, const std::string& tessEvalPath, const std::string& fragmentPath) {
    // Load and compile the vertex shader
    std::string vertexCode = readShaderFile(vertexPath);
    unsigned int vertexShaderId = compileShader(GL_VERTEX_SHADER, vertexCode);

    // Load and compile the tessellation control shader
    std::string tessControlCode = readShaderFile(tessControlPath);
    unsigned int tessControlShaderId = compileShader(GL_TESS_CONTROL_SHADER, tessControlCode);

    // Load and compile the tessellation evaluation shader
    std::string tessEvalCode = readShaderFile(tessEvalPath);
    unsigned int tessEvalShaderId = compileShader(GL_TESS_EVALUATION_SHADER, tessEvalCode);

    // Load and compile the fragment shader
    std::string fragmentCode = readShaderFile(fragmentPath);
    unsigned int fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    // Link the shader program
    linkProgram(vertexShaderId, 0, tessControlShaderId, tessEvalShaderId, fragmentShaderId);
}

void ShaderProgram::use() const {
    glUseProgram(programId);
}

void ShaderProgram::setBool(const std::string& name, bool value) const {
    glUniform1i(getUniformLocation(name), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int ShaderProgram::compileShader(unsigned int shaderType, const std::string& shaderCode) {
    unsigned int shaderId = glCreateShader(shaderType);
    const char* codePtr = shaderCode.c_str();
    glShaderSource(shaderId, 1, &codePtr, nullptr);
    glCompileShader(shaderId);

    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to compile shader: " + std::string(infoLog));
    }

    return shaderId;
}

void ShaderProgram::linkProgram(unsigned int vertexShaderId, unsigned int geometryShaderId, unsigned int tessControlShaderId, unsigned int tessEvalShaderId, unsigned int fragmentShaderId) {
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);

    if (geometryShaderId != 0) {
        glAttachShader(programId, geometryShaderId);
    }

    if (tessControlShaderId != 0 && tessEvalShaderId != 0) {
        glAttachShader(programId, tessControlShaderId);
        glAttachShader(programId, tessEvalShaderId);
    }

    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);

    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderId);

    if (geometryShaderId != 0) {
        glDeleteShader(geometryShaderId);
    }

    if (tessControlShaderId != 0 && tessEvalShaderId != 0) {
        glDeleteShader(tessControlShaderId);
        glDeleteShader(tessEvalShaderId);
    }

    glDeleteShader(fragmentShaderId);
}

GLuint ShaderProgram::getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(programId, name.c_str());
}

std::string ShaderProgram::readShaderFile(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}
