#include "glshaderprogram.hpp"

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
    wxVector<GLShader>::iterator it;
    
    for (it = m_shaders.begin(); it != m_shaders.end(); ++it)
    {
        it->Create();
        it->Compile();
    }
    
    m_id = glCreateProgram();
    for (it = m_shaders.begin(); it != m_shaders.end(); ++it)
    {
        glAttachShader(m_id, it->GetId());
    }
    
    Link();
    
    for (it = m_shaders.begin(); it != m_shaders.end(); ++it)
    {
        it->Delete();
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
        wxString errorMessage = "An error occurred when linking an OpenGL shader program:\n"+
                                wxString(infoLog);
        throw errorMessage;
    }
}