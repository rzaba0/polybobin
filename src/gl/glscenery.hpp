#ifndef GLSCENERY_HPP
#define GLSCENERY_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <glm/gtc/matrix_transform.hpp>
#include "globject.hpp"
#include "../map/pmsenums.hpp"
#include "../map/pmsstructs.hpp"

/**
 * \brief Handles the drawing of scenery.
 */
class GLScenery: GLObject
{
    public:
        void EditScenery(unsigned int sceneryIndex, PMSScenery newScenery);

        /**
         * \brief Renders all scenerys whose level is equal to targetLevel.
         */
        void Render(glm::mat4 transform, PMSSceneryLevel targetLevel);

        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();
        void SetupTextures(wxString sceneryDirectoryPath, wxVector<PMSSceneryType> sceneryTypes);
        void SetupVAO(wxVector<PMSScenery> sceneryInstances);

    private:
        wxVector<PMSScenery> m_sceneryInstances;
        wxVector<GLuint> m_sceneryTextures;
};

#endif