#pragma once

#include <glad/glad.h>


#include <string>
#include <vector>
#include <exception>
#include <memory>

class ShaderException : public std::exception
{
public:
    std::basic_string<GLchar> shaderLog;
    const char* what() const noexcept override { return "Unable to Compile/Link Shader"; }
    ShaderException(const std::basic_string<GLchar>& log) : shaderLog(log) {};
};

class Shader
{
    struct ShaderResource
    {
        GLuint ID;
        ShaderResource(const GLenum& shaderType) { ID = glCreateShader(shaderType); };
        ~ShaderResource() { glDeleteShader(ID); };
    };
    std::shared_ptr<ShaderResource> shader;
public:
    Shader(const char* sourceFile, const GLenum& shaderType);
    friend class Program;
};

class Program
{
    struct ProgramResource
    {
        GLuint ID;
        ProgramResource() { ID = glCreateProgram(); };
        ~ProgramResource() { glUseProgram(0); glDeleteProgram(ID); };
    };
    std::shared_ptr<ProgramResource> program;
public:
    Program(const std::vector<Shader>& shaders);
    GLint getUniformLocation(const char* loc) const;
    GLint getAttribLocation(const char* attrib) const;
    void use() const;
};

