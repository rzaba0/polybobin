#include "glselectionpolygons.hpp"
#include "../constants.hpp"
#include "image.hpp"
#include "../utils.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLSelectionPolygons::AddPolygon(PMSPolygonType polygonType, PMSVertex firstVertex)
{
    for (unsigned int i = 0; i < GL_SELECTION_POLYGON_VERTICES_COUNT; ++i)
    {
        EditPolygonVertex(m_polygonsCount, polygonType, i, firstVertex);
    }
    ++m_polygonsCount;
}

void GLSelectionPolygons::EditPolygonVertex(unsigned int polygonIndex, PMSPolygonType polygonType,
                                            unsigned int vertexIndex, PMSVertex newVertex)
{
    PMSColor color = Utils::GetPolygonColorByType(polygonType);
    GLfloat glVertex[] = {
        newVertex.x,
        newVertex.y,
        newVertex.z,
        (GLfloat)color.red / 255.0f,
        (GLfloat)color.green / 255.0f,
        (GLfloat)color.blue / 255.0f,
        0.5f,
        newVertex.x / m_textureWidth,
        newVertex.y / m_textureHeight
    };


    int offset = polygonIndex * GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES * GL_SELECTION_POLYGON_VERTICES_COUNT +
        vertexIndex * GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES, glVertex);
}

void GLSelectionPolygons::RenderSelected(const glm::mat4& transform, const PolygonSelection& selectedPolygons)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    for (const auto& poly : selectedPolygons)
    {
        glDrawArrays(GL_TRIANGLES, poly.id * 3, 3);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void GLSelectionPolygons::SetupShaderProgram()
{
    const GLchar *vertexShaderSource =
    {
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec4 color;\n"
        "layout (location = 2) in vec2 texture;\n"
        "uniform mat4 transform;\n"
        "out vec4 vertexColor;\n"
        "out vec2 vertexTexture;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = transform*(vec4(position, 1.0f));\n"
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

void GLSelectionPolygons::SetupTexture()
{
    Image textureImage;
    textureImage.OpenAndResize(PATH_GFX_SELECTION_GRID);

    m_textureWidth = textureImage.GetWidth();
    m_textureHeight = textureImage.GetHeight();

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

void GLSelectionPolygons::SetupVAO(wxVector<PMSPolygon> polygons)
{
    wxVector<GLfloat> vertices;
    m_polygonsCount = polygons.size();
    unsigned int i, j;

    if (m_polygonsCount > MAX_POLYGONS_COUNT)
    {
        m_polygonsCount = MAX_POLYGONS_COUNT;
    }

    for (i = 0; i < m_polygonsCount; ++i)
    {
        PMSColor color = Utils::GetPolygonColorByType(polygons[i].polygonType);

        for (j = 0; j < 3; ++j)
        {
            vertices.push_back(polygons[i].vertices[j].x);
            vertices.push_back(polygons[i].vertices[j].y);
            vertices.push_back(polygons[i].vertices[j].z);
            vertices.push_back((GLfloat) color.red / 255.0f);
            vertices.push_back((GLfloat) color.green / 255.0f);
            vertices.push_back((GLfloat) color.blue / 255.0f);
            vertices.push_back(0.5f);
            vertices.push_back(polygons[i].vertices[j].x / m_textureWidth);
            vertices.push_back(polygons[i].vertices[j].y / m_textureHeight);
        }
    }

    // Initialization of unused polygons.
    for (i = 0; i < MAX_POLYGONS_COUNT - m_polygonsCount; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            for (unsigned int k = 0; k < GL_SELECTION_POLYGON_VERTEX_SIZE; ++k)
            {
                vertices.push_back(0.0f);
            }
        }
    }
    
    glGenBuffers(1, &m_vbo);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES * GL_SELECTION_POLYGON_VERTICES_COUNT * MAX_POLYGONS_COUNT,
                &vertices[0], GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, GL_SELECTION_POLYGON_VERTEX_SIZE_BYTES, (GLvoid*)(7 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}
