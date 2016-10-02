#include "gloutlinepolygons.hpp"
#include "../../utils.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLOutlinePolygons::RenderAll(glm::mat4 transform)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(m_vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, m_polygonsVerticesCount);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
}

void GLOutlinePolygons::RenderSelected(glm::mat4 transform, wxVector<unsigned int> selectedPolygonsIds)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(m_vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (unsigned int i = 0; i < selectedPolygonsIds.size(); ++i)
    {
        glDrawArrays(GL_TRIANGLES, selectedPolygonsIds[i] * 3, 3);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
}

void GLOutlinePolygons::SetupVAO(wxVector<PMSPolygon> polygons)
{
    wxVector<GLfloat> vertices;
    unsigned int polygonsCount = polygons.size(), i, j;

    for (i = 0; i < polygonsCount; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            vertices.push_back((GLfloat)polygons[i].vertices[j].x);
            vertices.push_back((GLfloat)polygons[i].vertices[j].y);
            vertices.push_back((GLfloat)polygons[i].vertices[j].z);

            PMSColor color = Utils::GetPolygonColorByType(polygons[i].polygonType);
            vertices.push_back((GLfloat)color.red / 255.0f);
            vertices.push_back((GLfloat)color.green / 255.0f);
            vertices.push_back((GLfloat)color.blue / 255.0f);
            vertices.push_back((GLfloat)color.alpha / 255.0f);
        }
    }
    m_polygonsVerticesCount = polygonsCount * 3;

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, GL_OUTLINE_VERTEX_SIZE*m_polygonsVerticesCount, &vertices[0], GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_OUTLINE_VERTEX_SIZE, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_OUTLINE_VERTEX_SIZE, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}