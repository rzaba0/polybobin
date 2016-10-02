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
};

#endif