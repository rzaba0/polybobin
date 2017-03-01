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
        void ApplySelection(const PolygonSelection& selectedPolygons);
        void RenderSelected(const glm::mat4& transform, const PolygonSelection& selectedPolygonsIds);
        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();
        void SetupTexture();
        void SetupVAO(wxVector<PMSPolygon> polygons);

    private:
        void ApplyVertexAlpha(unsigned polygonIndex, unsigned vertexIndex, GLfloat alpha);

        unsigned int m_polygonsCount;
        GLuint m_texture, m_vbo;
        int m_textureWidth, m_textureHeight;

        static constexpr int GL_SELECTION_POLYGON_VERTEX_SIZE = 9;
        static constexpr int GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES = GL_SELECTION_POLYGON_VERTEX_SIZE * sizeof(GLfloat);
        static constexpr int GL_SELECTION_POLYGON_VERTICES_COUNT = 3;
        static constexpr GLfloat GL_SELECTION_ALPHA = 0.6f;
};