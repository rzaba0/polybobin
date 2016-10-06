#ifndef GLPOLYGONS_HPP
#define GLPOLYGONS_HPP

#include "globject.hpp"
#include "../map/pmsstructs.hpp"
#include <glm/gtc/matrix_transform.hpp>

class GLPolygons: GLObject
{
    public:
        void Render(glm::mat4 transform);
        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();
        void SetupTexture(wxString texturesDirectoryPath, wxString textureFilename);
        void SetupVAO(wxVector<PMSPolygon> polygons);

    private:
        int m_verticesCount;
        GLuint m_texture, m_vbo;

        static const int GL_POLYGON_VERTEX_SIZE = 9 * sizeof(GLfloat);
};

#endif