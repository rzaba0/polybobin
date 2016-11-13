#ifndef GLBACKGROUND_HPP
#define GLBACKGROUND_HPP

#include "globject.hpp"
#include "../map/map.hpp"
#include "../map/pmsstructs.hpp"
#include <glm/gtc/matrix_transform.hpp>

/**
 * \brief Handles the drawing of background square.
 */
class GLBackground: GLObject
{
    public:
        void Render(glm::mat4 transform);
        
        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();
        
        void SetupVAO(PMSColor backgroundTopColor, PMSColor backgroundBottomColor,
                      float *backgroundSquareBoundaries);

        void UpdateVBO(PMSColor backgroundTopColor, PMSColor backgroundBottomColor,
                       float *backgroundSquareBoundaries);

    private:
        GLuint m_vbo;

        static const int GL_BACKGROUND_VERTEX_SIZE = 7;
        static const int GL_BACKGROUND_VERTEX_SIZE_BYTES = GL_BACKGROUND_VERTEX_SIZE * sizeof(GLfloat);
        static const int GL_BACKGROUND_VERTICES_COUNT = 4;

        void SetupVerticesArray(GLfloat vertices[GL_BACKGROUND_VERTICES_COUNT][GL_BACKGROUND_VERTEX_SIZE],
                                PMSColor backgroundTopColor, PMSColor backgroundBottomColor,
                                float *backgroundSquareBoundaries);
};

#endif