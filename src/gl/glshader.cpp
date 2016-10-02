#include "glshader.hpp"
#include <wx/string.h>

GLShader::GLShader(GLenum type, const GLchar *code)
{
    m_id = 0;
    m_type = type;
    m_code = code;
}

void GLShader::Compile()
{
    GLint success;
    char infoLog[512];
    
    glCompileShader(m_id);
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(m_id, sizeof(infoLog), NULL, infoLog);
        wxString errorMessage = "An error occurred when compiling an OpenGL shader:\n"+
                                wxString(infoLog);
        throw errorMessage;
    }
}

void GLShader::Create()
{
    m_id = glCreateShader(m_type);
    glShaderSource(m_id, 1, &m_code, NULL);
}

void GLShader::Delete()
{
    glDeleteShader(m_id);
}

GLuint GLShader::GetId()
{
    return m_id;
}