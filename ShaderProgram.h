#pragma once

#include <string>
#include <GL/glew.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram {
public:
    ShaderProgram();
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
    ShaderProgram(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
    ShaderProgram(const std::string& vertexPath, const std::string& tessControlPath, const std::string& tessEvalPath, const std::string& fragmentPath);
    virtual ~ShaderProgram();

    void load(const std::string& vertexPath, const std::string& fragmentPath);
    void load(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
    void load(const std::string& vertexPath, const std::string& tessControlPath, const std::string& tessEvalPath, const std::string& fragmentPath);

    void use() const;
    void unuse() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    GLuint getUniformLocation(const std::string& name) const;

private:
    unsigned int programId;
    std::unordered_map<std::string, int> uniformLocations;

    unsigned int compileShader(unsigned int shaderType, const std::string& shaderCode);
    void linkProgram(unsigned int vertexShaderId, unsigned int geometryShaderId, unsigned int tessControlShaderId, unsigned int tessEvalShaderId, unsigned int fragmentShaderId);


    std::string readShaderFile(const std::string& filePath);
};
