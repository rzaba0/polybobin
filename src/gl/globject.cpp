#include "globject.hpp"

void GLObject::SetupShaderProgram(const GLchar *vertexShaderSource, const GLchar *fragmentShaderSource)
{
    m_shaderProgram.AddShader(GL_VERTEX_SHADER, vertexShaderSource);
    m_shaderProgram.AddShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_shaderProgram.Init();
}