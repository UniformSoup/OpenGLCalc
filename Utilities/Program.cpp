#include "Program.hpp"

#include <fstream>

GLint Program::getUniformLocation(const char* loc) const { return glGetUniformLocation(program->ID, loc); }
GLint Program::getAttribLocation(const char* attrib) const { return glGetAttribLocation(program->ID, attrib); }
void Program::use() const { glUseProgram(program->ID); }

Program::Program(const std::vector<Shader>& shaders)
    : program(std::make_shared<ProgramResource>()) 
{
    for (auto& s : shaders) glAttachShader(program->ID, s.shader->ID);
    
    glLinkProgram(program->ID);

    GLint isLinked = 0;
    glGetProgramiv(program->ID, GL_LINK_STATUS, &isLinked);

    if (isLinked == GL_FALSE)
    {
        GLint logSize = 0;
        glGetProgramiv(program->ID, GL_INFO_LOG_LENGTH, &logSize);

        std::basic_string<GLchar> errorLog;
        errorLog.resize(logSize);
        glGetProgramInfoLog(program->ID, logSize, &logSize, &errorLog[0]);
        errorLog.shrink_to_fit();

        throw ShaderException(errorLog);
    }
}

Shader::Shader(const char* sourceFile, const GLenum& shaderType)
    : shader(std::make_shared<ShaderResource>(shaderType))
{
    std::ifstream shaderFile(sourceFile); 
    
    if (!shaderFile.is_open()) throw std::runtime_error("Could not open " + std::string(sourceFile));

    shaderFile.seekg(0, std::ios::end);
    int size = shaderFile.tellg();
    shaderFile.seekg(0, std::ios::beg);

    std::string source;
    source.resize(size);
    shaderFile.read(&source[0], size);

    const auto pSource = &source[0];
    glShaderSource(shader->ID, 1, &pSource, nullptr);
    glCompileShader(shader->ID);

    GLint success = 0;
    glGetShaderiv(shader->ID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint logSize = 0;
        glGetShaderiv(shader->ID, GL_INFO_LOG_LENGTH, &logSize);

        std::basic_string<GLchar> errorLog;
        errorLog.resize(logSize);
        glGetShaderInfoLog(shader->ID, logSize, &logSize, &errorLog[0]);
        errorLog.shrink_to_fit();

        throw ShaderException(errorLog);
    }
}

