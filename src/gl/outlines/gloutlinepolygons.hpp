#ifndef GLOUTLINEPOLYGONS_HPP
#define GLOUTLINEPOLYGONS_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "gloutline.hpp"
#include "../../map/pmsenums.hpp"
#include "../../map/pmsstructs.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../../polygonselection.hpp"

/**
 * \brief Handles the drawing of polygons' outline.
 */
class GLOutlinePolygons: public GLOutline
{
    public:
        void AddPolygon(PMSPolygonType polygonType, PMSVertex firstVertex);
        void EditPolygonVertex(unsigned int polygonIndex, PMSPolygonType polygonType,
                               unsigned int vertexIndex, PMSVertex newVertex);
        void ApplySelection(const PolygonSelection& selectedPolygons);
        /**
         * \brief Renders outline for all polygons.
         */
        void RenderAll(const glm::mat4& transform);

        /**
         * \brief Renders outline for selected polygons.
         */
        void RenderSelected(const glm::mat4& transform, const PolygonSelection& selectedPolygonsIds);
        void SetupVAO(wxVector<PMSPolygon> polygons);

    private:
        void ApplyVertexAlpha(unsigned polygonIndex, unsigned vertexIndex, GLfloat alpha);
        GLuint m_vbo;
        unsigned int m_polygonsCount;

        static const int GL_OUTLINE_POLYGON_VERTICES_COUNT = 3;
};

#endif