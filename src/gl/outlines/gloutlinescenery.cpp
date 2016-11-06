#include "gloutlinescenery.hpp"
#include <glm/gtc/type_ptr.hpp>

GLOutlineScenery::GLOutlineScenery(PMSColor behindAllColor,
                                   PMSColor behindMapColor,
                                   PMSColor behindNoneColor)
{
    m_verticesColor[lBEHIND_ALL] = behindAllColor;
    m_verticesColor[lBEHIND_MAP] = behindMapColor;
    m_verticesColor[lBEHIND_NONE] = behindNoneColor;
}

void GLOutlineScenery::RenderAll(glm::mat4 transform)
{
    m_shaderProgram.Use();

    glBindVertexArray(m_vao);
    for (int i = 0; i < m_sceneryVerticesCount; i += RECTANGLE_VERTICES_COUNT)
    {
        glm::mat4 _transform = transform;

        _transform = glm::translate(_transform, glm::vec3(m_sceneryInstances[i / 4].x, m_sceneryInstances[i / 4].y, 0.0f));
        _transform = glm::rotate(_transform, -m_sceneryInstances[i / 4].rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        _transform = glm::scale(_transform, glm::vec3(m_sceneryInstances[i / 4].scaleX, m_sceneryInstances[i / 4].scaleY, 0.0f));

        glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                           1, GL_FALSE, glm::value_ptr(_transform));

        glDrawArrays(GL_LINE_LOOP, i, RECTANGLE_VERTICES_COUNT);
    }

    glBindVertexArray(0);
}

void GLOutlineScenery::RenderSelected(glm::mat4 transform, wxVector<unsigned int> selectedSceneryIds)
{
    m_shaderProgram.Use();

    glBindVertexArray(m_vao);
    for (unsigned int i = 0; i < selectedSceneryIds.size(); ++i)
    {
        glm::mat4 _transform = transform;

        int sceneryId = selectedSceneryIds[i];
        _transform = glm::translate(_transform, glm::vec3(m_sceneryInstances[sceneryId].x, m_sceneryInstances[sceneryId].y, 0.0f));
        _transform = glm::rotate(_transform, -m_sceneryInstances[sceneryId].rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        _transform = glm::scale(_transform, glm::vec3(m_sceneryInstances[sceneryId].scaleX, m_sceneryInstances[sceneryId].scaleY, 0.0f));
        
        glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                           1, GL_FALSE, glm::value_ptr(_transform));

        glDrawArrays(GL_LINE_LOOP, sceneryId * 4, RECTANGLE_VERTICES_COUNT);
    }

    glBindVertexArray(0);
}

void GLOutlineScenery::SetupVAO(wxVector<PMSScenery> sceneryInstances)
{
    wxVector<GLfloat> vertices;
    m_sceneryInstances = sceneryInstances;

    for (unsigned int i = 0; i < sceneryInstances.size(); ++i)
    {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        SetVertexColor(vertices, sceneryInstances[i].level);

        vertices.push_back(sceneryInstances[i].width);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        SetVertexColor(vertices, sceneryInstances[i].level);

        vertices.push_back(sceneryInstances[i].width);
        vertices.push_back(sceneryInstances[i].height);
        vertices.push_back(1.0f);    
        SetVertexColor(vertices, sceneryInstances[i].level);

        vertices.push_back(0.0f);
        vertices.push_back(sceneryInstances[i].height);
        vertices.push_back(1.0f);
        SetVertexColor(vertices, sceneryInstances[i].level);
    }
    m_sceneryVerticesCount = sceneryInstances.size() * 4;

    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, GL_OUTLINE_VERTEX_SIZE_BYTES*m_sceneryVerticesCount, &vertices[0], GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_OUTLINE_VERTEX_SIZE_BYTES, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_OUTLINE_VERTEX_SIZE_BYTES, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void GLOutlineScenery::SetVertexColor(wxVector<GLfloat> &vertices, int sceneryLevel)
{
    PMSColor vertexColor = m_verticesColor[sceneryLevel];

    vertices.push_back((GLfloat)vertexColor.red / 255.0f);
    vertices.push_back((GLfloat)vertexColor.green / 255.0f);
    vertices.push_back((GLfloat)vertexColor.blue / 255.0f);
    vertices.push_back((GLfloat)vertexColor.alpha / 255.0f);
}