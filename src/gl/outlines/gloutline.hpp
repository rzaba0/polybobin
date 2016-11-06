#ifndef GLOUTLINE_HPP
#define GLOUTLINE_HPP

#include "../globject.hpp"

/**
 * \brief Base class for drawing outlines.
 */
class GLOutline: protected GLObject
{
    public:
        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();

    protected:
        static const int GL_OUTLINE_VERTEX_SIZE = 7;
        static const int GL_OUTLINE_VERTEX_SIZE_BYTES = GL_OUTLINE_VERTEX_SIZE * sizeof(GLfloat);
};

#endif