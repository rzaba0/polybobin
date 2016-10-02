#ifndef GLSELECTIONPOLYGONS_HPP
#define GLSELECTIONPOLYGONS_HPP

#include <wx/vector.h>
#include "globject.hpp"
#include "../map/pmsenums.hpp"
#include "../map/pmsstructs.hpp"
#include <glm/gtc/matrix_transform.hpp>

/**
 * \brief Handles the drawing of grid on top of selected polygons.
 */
class GLSelectionPolygons: GLObject
{
    public:
        void RenderSelected(glm::mat4 transform, wxVector<unsigned int> selectedPolygonsIds);
        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();
        void SetupTexture();
        void SetupVAO(wxVector<PMSPolygon> polygons);

    private:
        GLuint m_texture;
        int m_textureWidth, m_textureHeight;

        static const int ELEMENTS_PER_VERTEX = 9;
        static const int GL_SELECTION_POLYGON_VERTEX_SIZE = ELEMENTS_PER_VERTEX * sizeof(GLfloat);

        void AdjustTextureCoordinates(wxVector<GLfloat> &vertices, int index);
        void AdjustVertexColor(wxVector<GLfloat> &vertices, int index, PMSPolygonType polygonType);
};

#endif