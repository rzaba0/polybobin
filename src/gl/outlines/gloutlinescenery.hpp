#ifndef GLOUTLINESCENERY_HPP
#define GLOUTLINESCENERY_HPP

#include "gloutline.hpp"
#include "../../map/pmsstructs.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../../selection.hpp"

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
        void ResetSceneries(wxVector<PMSScenery> sceneryInstances);

        /**
         * \brief Renders outline for all scenery.
         */
        void RenderAll(const glm::mat4& transform);

        /**
         * \brief Renders outline for selected scenery.
         */
        void RenderSelected(const glm::mat4& transform, const Selection& selectedSceneryIds);
        void SetupVAO(wxVector<PMSScenery> sceneryInstances);

    private:
        wxVector<PMSScenery> m_sceneryInstances;
        int m_sceneryVerticesCount;
        GLuint m_vbo;
        PMSColor m_verticesColor[3];

        static const int RECTANGLE_VERTICES_COUNT = 4;

        void SetVertexColor(wxVector<GLfloat> &vertices, int sceneryLevel);
        void GenerateGLBufferVertices(wxVector<PMSScenery> &sceneryInstances, wxVector<GLfloat> &vertices);
};

#endif
