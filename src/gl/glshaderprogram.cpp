#include "glshaderprogram.hpp"
#include <system_error>

void GLShaderProgram::AddShader(GLenum type, const GLchar *code)
{
    GLShader shader(type, code);
    m_shaders.push_back(shader);
}

GLuint GLShaderProgram::GetUniformLocation(const GLchar *name)
{
    return glGetUniformLocation(m_id, name);
}

void GLShaderProgram::Init()
{
    for (auto &shader : m_shaders)
    {
        shader.Create();
        shader.Compile();
    }
    
    m_id = glCreateProgram();
    for (auto &shader : m_shaders)
    {
        glAttachShader(m_id, shader.GetId());
    }
    
    Link();
    
    for (auto &shader : m_shaders)
    {
        shader.Delete();
    }
}

void GLShaderProgram::StopUse()
{
    glUseProgram(0);
}

void GLShaderProgram::Use()
{
    glUseProgram(m_id);
}

void GLShaderProgram::Link()
{
    GLint success;
    char infoLog[512];

    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(m_id, sizeof(infoLog), NULL, infoLog);
        throw std::runtime_error("An error occurred when linking an OpenGL shader program : \n" + std::string(infoLog));
    }
}