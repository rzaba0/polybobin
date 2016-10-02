#ifndef GLSHADER_HPP
#define GLSHADER_HPP

#include "glfunctions.hpp"

/**
 * \brief Generic OpenGL shader.
 */
class GLShader
{
    public:
        GLShader(GLenum type, const GLchar *code);
        void Compile();
        void Create();
        void Delete();
        GLuint GetId();

    private:
        GLuint m_id;
        GLenum m_type;
        const GLchar *m_code;
};

#endif