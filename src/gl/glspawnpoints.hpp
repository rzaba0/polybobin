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
        void AddSpawnPoint(PMSSpawnPoint spawnPoint);
        void EditSpawnPoint(unsigned int spawnPointIdx, PMSSpawnPoint spawnPoint);
        void ResetSpawnPoints(wxVector<PMSSpawnPoint> spawnPoints);

        void Render(glm::mat4 transform);
        using GLObject::SetupShaderProgram;
        void SetupShaderProgram();
        void SetupTextures();
        void SetupVAO(wxVector<PMSSpawnPoint> spawnPoints);

    private:
        GLuint m_texture;
        int m_spawnPointsCount;
        GLuint m_vbo, m_ebo;

        static const int GL_SPAWNPOINT_INDICES_SIZE = 6;
        static const int GL_SPAWNPOINT_INDICES_SIZE_BYTES = GL_SPAWNPOINT_INDICES_SIZE * sizeof(GLuint);
        static const int GL_SPAWNPOINT_VERTEX_SIZE = 4;
        static const int GL_SPAWNPOINT_VERTEX_SIZE_BYTES = GL_SPAWNPOINT_VERTEX_SIZE * sizeof(GLfloat);
        static const int GL_SPAWNPOINT_VERTICES_COUNT = 4;
        static const int GL_SPAWNPOINT_EBO_SIZE = GL_SPAWNPOINT_INDICES_SIZE_BYTES;
        static const int GL_SPAWNPOINT_VBO_SIZE = GL_SPAWNPOINT_VERTEX_SIZE_BYTES * GL_SPAWNPOINT_VERTICES_COUNT;

        // Each spawn point image is 32x32 pixels.
        const int SPAWNPOINT_TEXTURE_SIZE = 32;
        // Indicates how many 32x32 images can fit in 1024x32 image.
        const int MAX_SPAWNPOINT_TYPES_COUNT = 1024 / 32;
        void AddGLSpawnPoint(wxVector<GLfloat> &vertices, wxVector<GLuint> &indices, PMSSpawnPoint spawnPoint, int spawnPointIdx);
};

#endif
