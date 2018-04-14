#include "glscenery.hpp"
#include "image.hpp"
#include "../constants.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLScenery::EditScenery(unsigned int sceneryIndex, PMSScenery newScenery)
{
    if (m_sceneryInstances[sceneryIndex].color != newScenery.color
        || m_sceneryInstances[sceneryIndex].alpha != newScenery.alpha
        || m_sceneryInstances[sceneryIndex].width != newScenery.width
        || m_sceneryInstances[sceneryIndex].height != newScenery.height)
    {
        wxVector<GLfloat> vertices;

        for (int j = 0; j < 4; ++j)
        {
            vertices.push_back(0.0f); // These values will be set manually later.
            vertices.push_back(0.0f);
            vertices.push_back((GLfloat)newScenery.color.red / 255.0f);
            vertices.push_back((GLfloat)newScenery.color.green / 255.0f);
            vertices.push_back((GLfloat)newScenery.color.blue / 255.0f);
            vertices.push_back((GLfloat)newScenery.alpha / 255.0f);
            vertices.push_back(0.0f); // These values will be set manually later.
            vertices.push_back(0.0f);
        }
        vertices[0] = 0.0f;
        vertices[1] = 0.0f;
        vertices[6] = 0.0f;
        vertices[7] = 0.0f;

        vertices[8] = newScenery.width;
        vertices[9] = 0.0f;
        vertices[14] = 1.0f;
        vertices[15] = 0.0f;

        vertices[16] = 0.0f;
        vertices[17] = newScenery.height;
        vertices[22] = 0.0f;
        vertices[23] = 1.0f;

        vertices[24] = newScenery.width;
        vertices[25] = newScenery.height;
        vertices[30] = 1.0f;
        vertices[31] = 1.0f;

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sceneryIndex * 4 * 8, sizeof(GLfloat) * 4 * 8, &vertices[0]);

    }
    m_sceneryInstances[sceneryIndex] = newScenery;
}

void GLScenery::ResetSceneries(wxVector<PMSScenery> sceneryInstances)
{
    m_sceneryInstances = sceneryInstances;
    RemoveUnusedSceneryTypes();

    int sceneryInstancesCount = sceneryInstances.size();

    wxVector<GLfloat> vertices;
    wxVector<GLuint> indices;

    GenerateGLBufferVertices(sceneryInstances, vertices);
    GenerateGLBufferIndices(sceneryInstances, indices);

    if (vertices.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sceneryInstancesCount * 4 * 8, &vertices[0]);
    }
    if (indices.size() > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * 6 * sceneryInstancesCount, &indices[0]);
    }
}

void GLScenery::Render(glm::mat4 transform, PMSSceneryLevel targetLevel)
{
    m_shaderProgram.Use();
    glBindVertexArray(m_vao);

    for (unsigned int i = 0; i < m_sceneryInstances.size(); ++i)
    {
        if (m_sceneryInstances[i].level == targetLevel)
        {
            glm::mat4 _transform = transform;

            _transform = glm::translate(_transform, glm::vec3(m_sceneryInstances[i].x, m_sceneryInstances[i].y, 0.0f));
            _transform = glm::rotate(_transform, -m_sceneryInstances[i].rotation, glm::vec3(0.0f, 0.0f, 1.0f));
            _transform = glm::scale(_transform, glm::vec3(m_sceneryInstances[i].scaleX, m_sceneryInstances[i].scaleY, 0.0f));

            glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                               1, GL_FALSE, glm::value_ptr(_transform));
            glBindTexture(GL_TEXTURE_2D, m_sceneryTextures[m_sceneryInstances[i].style - 1]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(i * 6 * sizeof(GLuint)));
        }
    }
    glBindVertexArray(0);
}

void GLScenery::SetupShaderProgram()
{
    const GLchar *vertexShaderSource =
    {
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "layout (location = 1) in vec4 color;\n"
        "layout (location = 2) in vec2 texture;\n"
        "uniform mat4 transform;\n"
        "out vec4 vertexColor;\n"
        "out vec2 vertexTexture;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = transform*(vec4(position, 1.0f, 1.0f));\n"
        "   vertexColor = color;\n"
        "   vertexTexture = texture;\n"
        "}\n"
    };

    const GLchar *fragmentShaderSource =
    {
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "in vec2 vertexTexture;\n"
        "out vec4 color;\n"
        "uniform sampler2D ourTexture;\n"
        "void main()\n"
        "{\n"
        "   color = texture(ourTexture, vertexTexture)*vertexColor;\n"
        "}\n"
    };

    SetupShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void GLScenery::AddTexture(wxString sceneryPath)
{
    Image tmpImage;
    tmpImage.OpenAndResize(sceneryPath);

    m_sceneryTextures.push_back(0);
    glGenTextures(1, &m_sceneryTextures.back());
    glBindTexture(GL_TEXTURE_2D, m_sceneryTextures.back());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLint format = tmpImage.HasAlpha() ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format,
        tmpImage.GetWidth(), tmpImage.GetHeight(),
        0, format, GL_UNSIGNED_BYTE, tmpImage.GetData());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLScenery::SetupTextures(wxString sceneryDirectoryPath, wxVector<PMSSceneryType> sceneryTypes)
{
    for (unsigned int i = 0; i < sceneryTypes.size(); ++i)
    {
        AddTexture(sceneryDirectoryPath + sceneryTypes[i].name);
    }
}

void GLScenery::SetupVAO(wxVector<PMSScenery> sceneryInstances)
{
    m_sceneryInstances = sceneryInstances;

    wxVector<GLfloat> vertices;
    wxVector<GLuint> indices;

    GenerateGLBufferVertices(sceneryInstances, vertices);
    GenerateGLBufferIndices(sceneryInstances, indices);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_SCENERY_INSTANCES_COUNT * 4 * 8, &vertices[0], GL_STATIC_DRAW);
        }
        if (indices.size() > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6 * MAX_SCENERY_INSTANCES_COUNT, &indices[0], GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void GLScenery::GenerateGLBufferVertices(wxVector<PMSScenery> &sceneryInstances, wxVector<GLfloat> &vertices)
{
    unsigned int sceneryInstancesCount = sceneryInstances.size(), i, j;

    for (i = 0; i < sceneryInstancesCount; ++i)
    {
        for (j = 0; j < 4; ++j) // We create 4 vertices for every scenery.
        {
            vertices.push_back(0.0f); // These values will be set manually later.
            vertices.push_back(0.0f);
            vertices.push_back((GLfloat)sceneryInstances[i].color.red / 255.0f);
            vertices.push_back((GLfloat)sceneryInstances[i].color.green / 255.0f);
            vertices.push_back((GLfloat)sceneryInstances[i].color.blue / 255.0f);
            vertices.push_back((GLfloat)sceneryInstances[i].alpha / 255.0f);
            vertices.push_back(0.0f); // These values will be set manually later.
            vertices.push_back(0.0f);
        }
        vertices[i*8*4+0] = 0.0f;
        vertices[i*8*4+1] = 0.0f;
        vertices[i*8*4+6] = 0.0f;
        vertices[i*8*4+7] = 0.0f;

        vertices[i*8*4+8] = sceneryInstances[i].width;
        vertices[i*8*4+9] = 0.0f;
        vertices[i*8*4+14] = 1.0f;
        vertices[i*8*4+15] = 0.0f;

        vertices[i*8*4+16] = 0.0f;
        vertices[i*8*4+17] = sceneryInstances[i].height;
        vertices[i*8*4+22] = 0.0f;
        vertices[i*8*4+23] = 1.0f;

        vertices[i*8*4+24] = sceneryInstances[i].width;
        vertices[i*8*4+25] = sceneryInstances[i].height;
        vertices[i*8*4+30] = 1.0f;
        vertices[i*8*4+31] = 1.0f;
    }

    for (i = 0; i < MAX_SCENERY_INSTANCES_COUNT - sceneryInstancesCount; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }
}

void GLScenery::GenerateGLBufferIndices(wxVector<PMSScenery> &sceneryInstances, wxVector<GLuint> &indices)
{
    unsigned int sceneryInstancesCount = sceneryInstances.size(), i;

    for (i = 0; i < sceneryInstancesCount; ++i)
    {
        indices.push_back(i*4+0);
        indices.push_back(i*4+1);
        indices.push_back(i*4+2);
        indices.push_back(i*4+1);
        indices.push_back(i*4+3);
        indices.push_back(i*4+2);
    }

    for (i = sceneryInstancesCount; i < MAX_SCENERY_INSTANCES_COUNT; ++i)
    {
        indices.push_back(i * 4 + 0);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 3);
        indices.push_back(i * 4 + 2);
    }
}

void GLScenery::RemoveUnusedSceneryTypes()
{
    wxVector<int> useCount(m_sceneryTextures.size(), 0);
    wxVector<unsigned int> newIndexes(m_sceneryTextures.size(), 0);
    for (const auto& scenery : m_sceneryInstances)
    {
        useCount[scenery.style - 1]++;
    }

    int removeCount = 0;
    for (int i = 0; i < m_sceneryTextures.size(); i++)
    {
        if (useCount[i] == 0)
        {
            removeCount++;
        }
    }

    unsigned int swapsCount = 0;
    for (unsigned int i = 0; i < m_sceneryTextures.size(); ++i)
    {
        if (swapsCount + 1 <= removeCount && useCount[i] == 0)
        {
            ++swapsCount;
        }
        else
        {
            wxSwap(m_sceneryTextures[i], m_sceneryTextures[i - swapsCount]);
            newIndexes[i] = i - swapsCount;
        }
    }
    while (swapsCount--)
    {
        glDeleteTextures(1, &m_sceneryTextures.back());
        m_sceneryTextures.pop_back();
    }

    for (auto& scenery : m_sceneryInstances)
    {
        scenery.style = newIndexes[scenery.style - 1] + 1;
    }
}
