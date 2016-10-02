#ifndef GLSPAWNPOINTS_HPP
#define GLSPAWNPOINTS_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <glm/gtc/matrix_transform.hpp>
#include "globject.hpp"
#include "../map/pmsstructs.hpp"

/**
 * \brief Handles the drawing of spawn points.
 */
class GLSpawnPoints : GLObject
{
    public:
        void Render(glm::mat4 transform);
        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();
        void SetupTextures();
        void SetupVAO(wxVector<PMSSpawnPoint> spawnPoints);

    private:
        GLuint m_texture;
        int m_spawnPointsCount;
};

#endif