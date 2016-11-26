#ifndef GLOUTLINESCENERY_HPP
#define GLOUTLINESCENERY_HPP

#include "gloutline.hpp"
#include "../../map/pmsstructs.hpp"
#include <glm/gtc/matrix_transform.hpp>

/**
 * \brief Handles the drawing of scenery's outline. Can be used for selection and wireframe.
 */
class GLOutlineScenery: public GLOutline
{
    public:
        GLOutlineScenery(PMSColor behindAllColor,
                         PMSColor behindMapColor,
                         PMSColor behindNoneColor);

        void EditScenery(unsigned int sceneryIndex, PMSScenery scenery);
        
        /**
         * \brief Renders outline for all scenery.
         */
        void RenderAll(glm::mat4 transform);

        /**
         * \brief Renders outline for selected scenery.
         */
        void RenderSelected(glm::mat4 transform, wxVector<unsigned int> selectedSceneryIds);
        void SetupVAO(wxVector<PMSScenery> sceneryInstances);

    private:
        wxVector<PMSScenery> m_sceneryInstances;
        int m_sceneryVerticesCount;
        GLuint m_vbo;
        PMSColor m_verticesColor[3];

        static const int RECTANGLE_VERTICES_COUNT = 4;

        void SetVertexColor(wxVector<GLfloat> &vertices, int sceneryLevel);
};

#endif
