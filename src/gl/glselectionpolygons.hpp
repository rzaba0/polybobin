#pragma once

#include "globject.hpp"
#include "../map/pmsenums.hpp"
#include "../map/pmsstructs.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../polygonselection.hpp"

/**
 * \brief Handles the drawing of grid on top of selected polygons.
 */
class GLSelectionPolygons: GLObject
{
    public:
        void AddPolygon(PMSPolygonType polygonType, PMSVertex firstVertex);
        void EditPolygonVertex(unsigned int polygonIndex, PMSPolygonType polygonType,
                               unsigned int vertexIndex, PMSVertex newVertex);

        void RenderSelected(const glm::mat4& transform, const PolygonSelection& selectedPolygonsIds);
        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();
        void SetupTexture();
        void SetupVAO(wxVector<PMSPolygon> polygons);

    private:
        unsigned int m_polygonsCount;
        GLuint m_texture, m_vbo;
        int m_textureWidth, m_textureHeight;

        static const int GL_SELECTION_POLYGON_VERTEX_SIZE = 9;
        static const int GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES = GL_SELECTION_POLYGON_VERTEX_SIZE * sizeof(GLfloat);
        static const int GL_SELECTION_POLYGON_VERTICES_COUNT = 3;
};