#include "gloutlinepolygons.hpp"
#include "../../utils.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLOutlinePolygons::AddPolygon(PMSPolygonType polygonType, PMSVertex firstVertex)
{
    for (unsigned int i = 0; i < GL_OUTLINE_POLYGON_VERTICES_COUNT; ++i)
    {
        EditPolygonVertex(m_polygonsCount, polygonType, i, firstVertex);
    }
    ++m_polygonsCount;
}

void GLOutlinePolygons::EditPolygonVertex(unsigned int polygonIndex, PMSPolygonType polygonType,
                                          unsigned int vertexIndex, PMSVertex newVertex)
{
    wxVector<GLfloat> glVertex;
    glVertex.push_back(newVertex.x);
    glVertex.push_back(newVertex.y);
    glVertex.push_back(newVertex.z);

    PMSColor color = Utils::GetPolygonColorByType(polygonType);
    glVertex.push_back((GLfloat)color.red / 255.0f);
    glVertex.push_back((GLfloat)color.green / 255.0f);
    glVertex.push_back((GLfloat)color.blue / 255.0f);
    glVertex.push_back((GLfloat)color.alpha / 255.0f);

    const int offset = polygonIndex * GL_OUTLINE_VERTEX_SIZE_BYTES * GL_OUTLINE_POLYGON_VERTICES_COUNT +
        vertexIndex * GL_OUTLINE_VERTEX_SIZE_BYTES;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, GL_OUTLINE_VERTEX_SIZE_BYTES, &glVertex[0]);
}

void GLOutlinePolygons::ApplyVertexAlpha(unsigned polygonIndex, unsigned vertexIndex, GLfloat alpha)
{
    constexpr int alphaOffset = 6 * sizeof(GLfloat);
    const int offset = polygonIndex * GL_OUTLINE_VERTEX_SIZE_BYTES * GL_OUTLINE_POLYGON_VERTICES_COUNT +
        vertexIndex * GL_OUTLINE_VERTEX_SIZE_BYTES + alphaOffset;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(GLfloat), &alpha);
}

void GLOutlinePolygons::ResetPolygons(wxVector<PMSPolygon> polygons)
{
    wxVector<GLfloat> vertices;
    GenerateGLBufferVertices(polygons, vertices);
    m_polygonsCount = polygons.size();

    if (vertices.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
            GL_OUTLINE_VERTEX_SIZE_BYTES * GL_OUTLINE_POLYGON_VERTICES_COUNT * MAX_POLYGONS_COUNT, &vertices[0]);
    }
}

void GLOutlinePolygons::ApplySelection(const PolygonSelection& selectedPolygons)
{
    for (const auto& poly : selectedPolygons)
        for (unsigned i = 0; i < 3; i++)
            ApplyVertexAlpha(poly.id, i, poly.vertex[i] ? 1.0f : 0.0f);
}

void GLOutlinePolygons::RenderAll(const glm::mat4& transform)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(m_vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, m_polygonsCount * 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
}

void GLOutlinePolygons::RenderSelected(const glm::mat4& transform, const PolygonSelection& selectedPolygons)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(m_vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (const auto& poly : selectedPolygons)
    {
        glDrawArrays(GL_TRIANGLES, poly.id * 3, 3);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
}

void GLOutlinePolygons::SetupVAO(wxVector<PMSPolygon> polygons)
{
    m_polygonsCount = polygons.size();
    wxVector<GLfloat> vertices;
    GenerateGLBufferVertices(polygons, vertices);

    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, GL_OUTLINE_VERTEX_SIZE_BYTES * GL_OUTLINE_POLYGON_VERTICES_COUNT * MAX_POLYGONS_COUNT,
                &vertices[0], GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_OUTLINE_VERTEX_SIZE_BYTES, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_OUTLINE_VERTEX_SIZE_BYTES, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void GLOutlinePolygons::GenerateGLBufferVertices(wxVector<PMSPolygon> &polygons, wxVector<GLfloat> &vertices)
{
    unsigned int polygonsCount = polygons.size();
    unsigned int i, j;

    // Trim down polygons' count.
    if (polygonsCount > MAX_POLYGONS_COUNT)
    {
        polygonsCount = MAX_POLYGONS_COUNT;
    }

    for (i = 0; i < polygonsCount; ++i)
    {
        PMSColor color = Utils::GetPolygonColorByType(polygons[i].polygonType);

        for (j = 0; j < 3; ++j)
        {
            vertices.push_back((GLfloat)polygons[i].vertices[j].x);
            vertices.push_back((GLfloat)polygons[i].vertices[j].y);
            vertices.push_back((GLfloat)polygons[i].vertices[j].z);

            vertices.push_back((GLfloat)color.red / 255.0f);
            vertices.push_back((GLfloat)color.green / 255.0f);
            vertices.push_back((GLfloat)color.blue / 255.0f);
            vertices.push_back((GLfloat)color.alpha / 255.0f);
        }
    }

    // Initialization of unused polygons.
    for (i = 0; i < MAX_POLYGONS_COUNT - polygonsCount; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            for (unsigned int k = 0; k < GL_OUTLINE_VERTEX_SIZE; ++k)
            {
                vertices.push_back(0.0f);
            }
        }
    }
}