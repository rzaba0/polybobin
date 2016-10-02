#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include "glshaderprogram.hpp"

/**
 * \brief Generic OpenGL object; it helps recycling the code a bit.
 */
class GLObject
{
    public:
        void SetupShaderProgram(const GLchar *vertexShaderSource, const GLchar *fragmentShaderSource);

    protected:
        GLShaderProgram m_shaderProgram;
        GLuint m_vao;
};

#endif