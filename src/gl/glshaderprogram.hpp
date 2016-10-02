#ifndef GLSHADERPROGRAM_HPP
#define GLSHADERPROGRAM_HPP

#include <wx/vector.h>
#include "glshader.hpp"

/**
 * \brief Generic OpenGL shader program.
 */
class GLShaderProgram
{
    public:
        void AddShader(GLenum type, const GLchar *code);
        GLuint GetUniformLocation(const GLchar *name);
        void Init();
        void StopUse();
        void Use();

    private:
        int m_id;
        wxVector<GLShader> m_shaders;

        void Link();
};

#endif