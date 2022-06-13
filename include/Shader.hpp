#pragma once

#include "GlobalHead.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // 构造器读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void setBool(const char* name, bool value) const;  
    void setInt(const char* name, int value) const;   
    void setFloat(const char* name, float value) const;
    void setVec3(const char* name, float value1, float value2, float value3) const;
    void setVec3(const char* name, const glm::vec3& vec3) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setMat4(const char* name, const glm::mat4& mat4) const;
private:
    enum ErrorType
    {
        PROGRAM,
        SHADER,
    };
    void CheckCompileErrors(GLuint ID, ErrorType type);
private:
    GLuint ID;
};