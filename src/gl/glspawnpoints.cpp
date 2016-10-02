#include "glspawnpoints.hpp"
#include "../constants.hpp"
#include "image.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLSpawnPoints::Render(glm::mat4 transform)
{
    m_shaderProgram.Use();
    glBindVertexArray(m_vao);

    for (int i = 0; i < m_spawnPointsCount; ++i)
    {
        glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                           1, GL_FALSE, glm::value_ptr(transform));
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(i * 6 * sizeof(GLuint)));
    }
    glBindVertexArray(0);
}

void GLSpawnPoints::SetupShaderProgram()
{
    const GLchar *vertexShaderSource =
    {
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "layout (location = 1) in vec2 texture;\n"
        "uniform mat4 transform;\n"
        "out vec2 vertexTexture;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = transform*(vec4(position, 1.0f, 1.0f));\n"
        "   vertexTexture = texture;\n"
        "}\n"
    };

    const GLchar *fragmentShaderSource =
    {
        "#version 330 core\n"
        "in vec2 vertexTexture;\n"
        "out vec4 color;\n"
        "uniform sampler2D ourTexture;\n"
        "void main()\n"
        "{\n"
        "   color = texture(ourTexture, vertexTexture);\n"
        "}\n"
    };

    SetupShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void GLSpawnPoints::SetupTextures()
{
    Image textureImage;
    textureImage.OpenAndResize(PATH_GFX_SPAWNPOINTS);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLint format = textureImage.HasAlpha() ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 textureImage.GetWidth(), textureImage.GetHeight(),
                 0, format, GL_UNSIGNED_BYTE, textureImage.GetData());

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLSpawnPoints::SetupVAO(wxVector<PMSSpawnPoint> spawnPoints)
{
    m_spawnPointsCount = spawnPoints.size();
    wxVector<GLfloat> vertices;
    wxVector<GLuint> indices;

    for (int i = 0; i < m_spawnPointsCount; ++i)
    {
        // Each spawn point image is 32x32 pixels.
        const int SPAWNPOINT_TEXTURE_SIZE = 32;
        // Indicates how many 32x32 images can fit in 1024x32 image.
        const int MAX_SPAWNPOINT_TYPES_COUNT = 1024 / 32;

        // We want the image to be centered at spawn point's coordinates.
        spawnPoints[i].x -= SPAWNPOINT_TEXTURE_SIZE / 2;
        spawnPoints[i].y -= SPAWNPOINT_TEXTURE_SIZE / 2;

        vertices.push_back((GLfloat)spawnPoints[i].x);
        vertices.push_back((GLfloat)spawnPoints[i].y);
        vertices.push_back(0.0f);
        vertices.push_back((GLfloat)spawnPoints[i].type / (GLfloat)MAX_SPAWNPOINT_TYPES_COUNT);

        vertices.push_back((GLfloat)spawnPoints[i].x + (GLfloat)SPAWNPOINT_TEXTURE_SIZE);
        vertices.push_back((GLfloat)spawnPoints[i].y);
        vertices.push_back(1.0f);
        vertices.push_back((GLfloat)spawnPoints[i].type / (GLfloat)MAX_SPAWNPOINT_TYPES_COUNT);

        vertices.push_back((GLfloat)spawnPoints[i].x);
        vertices.push_back((GLfloat)spawnPoints[i].y + (GLfloat)SPAWNPOINT_TEXTURE_SIZE);
        vertices.push_back(0.0f);
        vertices.push_back((GLfloat)(spawnPoints[i].type+1) / (GLfloat)MAX_SPAWNPOINT_TYPES_COUNT);

        vertices.push_back((GLfloat)spawnPoints[i].x + (GLfloat)SPAWNPOINT_TEXTURE_SIZE);
        vertices.push_back((GLfloat)spawnPoints[i].y + (GLfloat)SPAWNPOINT_TEXTURE_SIZE);
        vertices.push_back(1.0f);
        vertices.push_back((GLfloat)(spawnPoints[i].type+1) / (GLfloat)MAX_SPAWNPOINT_TYPES_COUNT);

        indices.push_back(i*4+0);
        indices.push_back(i*4+1);
        indices.push_back(i*4+2);
        indices.push_back(i*4+1);
        indices.push_back(i*4+3);
        indices.push_back(i*4+2);
    }

    GLuint vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_spawnPointsCount * 4 * 4, &vertices[0], GL_STATIC_DRAW);
        }
        if (indices.size() > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_spawnPointsCount * 6, &indices[0], GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}