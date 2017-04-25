#include "glspawnpoints.hpp"
#include "../constants.hpp"
#include "image.hpp"
#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(i) (static_cast<char*>(0) + (i))

void GLSpawnPoints::AddSpawnPoint(PMSSpawnPoint spawnPoint)
{
    EditSpawnPoint(m_spawnPointsCount, spawnPoint);
    ++m_spawnPointsCount;
}

void GLSpawnPoints::EditSpawnPoint(unsigned int spawnPointIdx, PMSSpawnPoint spawnPoint)
{
    wxVector<GLfloat> vertices;
    wxVector<GLuint> indices;

    AddGLSpawnPoint(vertices, indices, spawnPoint, spawnPointIdx);

    const int vboOffset = spawnPointIdx * GL_SPAWNPOINT_VBO_SIZE;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, vboOffset, GL_SPAWNPOINT_VBO_SIZE, &vertices[0]);

    const int eboOffset = spawnPointIdx * GL_SPAWNPOINT_EBO_SIZE;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, eboOffset, GL_SPAWNPOINT_EBO_SIZE, &indices[0]);
}

void GLSpawnPoints::ResetSpawnPoints(wxVector<PMSSpawnPoint> spawnPoints)
{
    m_spawnPointsCount = spawnPoints.size();
    if (m_spawnPointsCount == 0)
    {
        return;
    }

    wxVector<GLfloat> vertices;
    wxVector<GLuint> indices;

    if (m_spawnPointsCount > MAX_SPAWNPOINTS_COUNT)
    {
        m_spawnPointsCount = MAX_SPAWNPOINTS_COUNT;
    }

    for (int i = 0; i < m_spawnPointsCount; ++i)
    {
        AddGLSpawnPoint(vertices, indices, spawnPoints[i], i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, GL_SPAWNPOINT_VBO_SIZE, &vertices[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, GL_SPAWNPOINT_EBO_SIZE, &indices[0]);
}

void GLSpawnPoints::Render(glm::mat4 transform)
{
    m_shaderProgram.Use();
    glBindVertexArray(m_vao);

    for (int i = 0; i < m_spawnPointsCount; ++i)
    {
        glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                           1, GL_FALSE, glm::value_ptr(transform));
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glDrawElements(GL_TRIANGLES, GL_SPAWNPOINT_INDICES_SIZE, GL_UNSIGNED_INT,
            BUFFER_OFFSET(i * GL_SPAWNPOINT_INDICES_SIZE_BYTES));
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

    if (m_spawnPointsCount > MAX_SPAWNPOINTS_COUNT)
    {
        m_spawnPointsCount = MAX_SPAWNPOINTS_COUNT;
    }

    for (int i = 0; i < m_spawnPointsCount; ++i)
    {
        AddGLSpawnPoint(vertices, indices, spawnPoints[i], i);
    }

    // Initialization of unused spawnpoints.
    vertices.resize(MAX_SPAWNPOINTS_COUNT * GL_SPAWNPOINT_VERTICES_COUNT * GL_SPAWNPOINT_VERTEX_SIZE, 0.0f);
    indices.resize(MAX_SPAWNPOINTS_COUNT * GL_SPAWNPOINT_INDICES_SIZE, 0);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, GL_SPAWNPOINT_VERTEX_SIZE_BYTES * MAX_SPAWNPOINTS_COUNT * GL_SPAWNPOINT_VERTICES_COUNT,
                &vertices[0], GL_DYNAMIC_DRAW);
        }
        if (indices.size() > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, GL_SPAWNPOINT_INDICES_SIZE_BYTES * MAX_SPAWNPOINTS_COUNT,
                &indices[0], GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, GL_SPAWNPOINT_VERTEX_SIZE_BYTES, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, GL_SPAWNPOINT_VERTEX_SIZE_BYTES, (GLvoid*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void GLSpawnPoints::AddGLSpawnPoint(wxVector<GLfloat> &vertices, wxVector<GLuint> &indices, PMSSpawnPoint spawnPoint, int spawnPointIdx)
{
    // We want the image to be centered at spawn point's coordinates.
    spawnPoint.x -= SPAWNPOINT_TEXTURE_SIZE / 2;
    spawnPoint.y -= SPAWNPOINT_TEXTURE_SIZE / 2;

    vertices.push_back((GLfloat)spawnPoint.x);
    vertices.push_back((GLfloat)spawnPoint.y);
    vertices.push_back(0.0f);
    vertices.push_back((GLfloat)spawnPoint.type / (GLfloat)MAX_SPAWNPOINT_TYPES_COUNT);

    vertices.push_back((GLfloat)spawnPoint.x + (GLfloat)SPAWNPOINT_TEXTURE_SIZE);
    vertices.push_back((GLfloat)spawnPoint.y);
    vertices.push_back(1.0f);
    vertices.push_back((GLfloat)spawnPoint.type / (GLfloat)MAX_SPAWNPOINT_TYPES_COUNT);

    vertices.push_back((GLfloat)spawnPoint.x);
    vertices.push_back((GLfloat)spawnPoint.y + (GLfloat)SPAWNPOINT_TEXTURE_SIZE);
    vertices.push_back(0.0f);
    vertices.push_back((GLfloat)(spawnPoint.type+1) / (GLfloat)MAX_SPAWNPOINT_TYPES_COUNT);

    vertices.push_back((GLfloat)spawnPoint.x + (GLfloat)SPAWNPOINT_TEXTURE_SIZE);
    vertices.push_back((GLfloat)spawnPoint.y + (GLfloat)SPAWNPOINT_TEXTURE_SIZE);
    vertices.push_back(1.0f);
    vertices.push_back((GLfloat)(spawnPoint.type+1) / (GLfloat)MAX_SPAWNPOINT_TYPES_COUNT);

    indices.push_back(spawnPointIdx * GL_SPAWNPOINT_VERTICES_COUNT + 0);
    indices.push_back(spawnPointIdx * GL_SPAWNPOINT_VERTICES_COUNT + 1);
    indices.push_back(spawnPointIdx * GL_SPAWNPOINT_VERTICES_COUNT + 2);
    indices.push_back(spawnPointIdx * GL_SPAWNPOINT_VERTICES_COUNT + 1);
    indices.push_back(spawnPointIdx * GL_SPAWNPOINT_VERTICES_COUNT + 3);
    indices.push_back(spawnPointIdx * GL_SPAWNPOINT_VERTICES_COUNT + 2);
}
