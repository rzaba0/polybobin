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

/**
 * \brief Handles the drawing of polygons' outline.
 */
class GLOutlinePolygons: public GLOutline
{
    public:
        void AddPolygon(PMSVertex firstVertex);
        void EditPolygonVertex(unsigned int polygonIndex, unsigned int vertexIndex, PMSVertex newVertex);

        /**
         * \brief Renders outline for all polygons.
         */
        void RenderAll(glm::mat4 transform);

        /**
         * \brief Renders outline for selected polygons.
         */
        void RenderSelected(glm::mat4 transform, wxVector<unsigned int> selectedPolygonsIds);
        void SetupVAO(wxVector<PMSPolygon> polygons);

    private:
        GLuint m_vbo;
        unsigned int m_polygonsCount;

        static const int GL_OUTLINE_POLYGON_VERTICES_COUNT = 3;
};

#endif