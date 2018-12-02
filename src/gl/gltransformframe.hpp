#ifndef GLTRANSFORMFRAME_HPP
#define GLTRANSFORMFRAME_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "globject.hpp"
#include <glm/gtc/matrix_transform.hpp>

class GLTransformFrame : protected GLObject
{
        using GLObject::SetupShaderProgram;
    public:
        void Move(const wxPoint bottomLeft,
                  const wxPoint bottomRight,
                  const wxPoint topLeft,
                  const wxPoint topRight);

        void Render(const glm::mat4& transform, const glm::mat4& translation);
        void SetupVAO();

        void SetupShaderProgram();

    private:
        const int GL_VERTEX_SIZE = 7;
        const int GL_FRAME_BYTES = GL_VERTEX_SIZE * sizeof(GLfloat);
        const int GL_ORIGIN_SIZE = 7 * 6;
        const int GL_ORIGIN_BYTES = GL_ORIGIN_SIZE * sizeof(GLfloat);

        GLuint m_vbo;

        wxPoint m_bottomLeft;
        wxPoint m_bottomRight;
        wxPoint m_topLeft;
        wxPoint m_topRight;

        void GenerateGLBufferVertices(wxVector<GLfloat> &vertices);
};

#endif // GLTRANSFORMFRAME_HPP


