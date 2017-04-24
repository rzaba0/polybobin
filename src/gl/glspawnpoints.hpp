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
class GLSpawnPoints: GLObject
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

        static const int GL_SPAWNPOINT_INDICES_SIZE = 6;
        static const int GL_SPAWNPOINT_INDICES_SIZE_BYTES = GL_SPAWNPOINT_INDICES_SIZE * sizeof(GLuint);
        static const int GL_SPAWNPOINT_VERTEX_SIZE = 4;
        static const int GL_SPAWNPOINT_VERTEX_SIZE_BYTES = GL_SPAWNPOINT_VERTEX_SIZE * sizeof(GLfloat);
        static const int GL_SPAWNPOINT_VERTICES_COUNT = 4;
};

#endif
