#include "glpolygons.hpp"
#include "image.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLPolygons::AddPolygon(PMSVertex firstVertex)
{
    for (unsigned int i = 0; i < GL_POLYGON_VERTICES_COUNT; ++i)
    {
        EditPolygonVertex(m_polygonsCount, i, firstVertex);
    }
    ++m_polygonsCount;
}

void GLPolygons::EditPolygonVertex(unsigned int polygonIndex, unsigned int vertexIndex,
                                   PMSVertex newVertex)
{
    GLfloat glVertex[] = {
        newVertex.x,
        newVertex.y,
        newVertex.z,
        newVertex.color.red / 255.0f,
        newVertex.color.green / 255.0f,
        newVertex.color.blue / 255.0f,
        newVertex.color.alpha / 255.0f,
        newVertex.textureS,
        newVertex.textureT
    };

    const int offset = polygonIndex * GL_POLYGON_VERTEX_SIZE_BYTES * GL_POLYGON_VERTICES_COUNT +
                 vertexIndex * GL_POLYGON_VERTEX_SIZE_BYTES;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, GL_POLYGON_VERTEX_SIZE_BYTES, glVertex);
}

void GLPolygons::ResetPolygons(wxVector<PMSPolygon> polygons)
{
    wxVector<GLfloat> vertices;
    GenerateGLBufferVertices(polygons, vertices);
    m_polygonsCount = polygons.size();

    if (vertices.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
            GL_POLYGON_VERTEX_SIZE_BYTES * MAX_POLYGONS_COUNT * GL_POLYGON_VERTICES_COUNT, &vertices[0]);
    }
}

unsigned int GLPolygons::GetTextureWidth()
{
    return m_textureWidth;
}

unsigned int GLPolygons::GetTextureHeight()
{
    return m_textureHeight;
}

void GLPolygons::Render(glm::mat4 transform)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_polygonsCount * 3);
    glBindVertexArray(0);
}

void GLPolygons::ReplaceTexture(wxString texturesDirectoryPath, wxString textureFilename)
{
    Image textureImage;
    textureImage.OpenAndResize(texturesDirectoryPath + textureFilename);
    m_textureWidth = (unsigned int) textureImage.GetWidth();
    m_textureHeight = (unsigned int) textureImage.GetHeight();

    glBindTexture(GL_TEXTURE_2D, m_texture);
    GLint format = textureImage.HasAlpha() ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 m_textureWidth, m_textureHeight,
                 0, format, GL_UNSIGNED_BYTE, textureImage.GetData());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLPolygons::SetupShaderProgram()
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

void GLPolygons::SetupTexture(wxString texturesDirectoryPath, wxString textureFilename)
{
    Image textureImage;
    textureImage.OpenAndResize(texturesDirectoryPath + textureFilename);
    m_textureWidth = (unsigned int) textureImage.GetWidth();
    m_textureHeight = (unsigned int) textureImage.GetHeight();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint format = textureImage.HasAlpha() ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 textureImage.GetWidth(), textureImage.GetHeight(),
                 0, format, GL_UNSIGNED_BYTE, textureImage.GetData());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLPolygons::SetupVAO(wxVector<PMSPolygon> polygons)
{
    wxVector<GLfloat> vertices;
    GenerateGLBufferVertices(polygons, vertices);
    m_polygonsCount = polygons.size();

    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            // We initialize all polygons (not only those present in map).
            glBufferData(GL_ARRAY_BUFFER, GL_POLYGON_VERTEX_SIZE_BYTES * MAX_POLYGONS_COUNT * GL_POLYGON_VERTICES_COUNT,
                &vertices[0], GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_POLYGON_VERTEX_SIZE_BYTES, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_POLYGON_VERTEX_SIZE_BYTES, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, GL_POLYGON_VERTEX_SIZE_BYTES, (GLvoid*)(7 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void GLPolygons::GenerateGLBufferVertices(wxVector<PMSPolygon> &polygons, wxVector<GLfloat> &vertices)
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
        for (j = 0; j < 3; ++j)
        {
            vertices.push_back((GLfloat)polygons[i].vertices[j].x);
            vertices.push_back((GLfloat)polygons[i].vertices[j].y);
            vertices.push_back((GLfloat)polygons[i].vertices[j].z);
            vertices.push_back((GLfloat)polygons[i].vertices[j].color.red / 255.0);
            vertices.push_back((GLfloat)polygons[i].vertices[j].color.green / 255.0);
            vertices.push_back((GLfloat)polygons[i].vertices[j].color.blue / 255.0);
            vertices.push_back((GLfloat)polygons[i].vertices[j].color.alpha / 255.0);
            vertices.push_back((GLfloat)polygons[i].vertices[j].textureS);
            vertices.push_back((GLfloat)polygons[i].vertices[j].textureT);
        }
    }

    // Initialization of unused polygons.
    for (i = 0; i < MAX_POLYGONS_COUNT - polygonsCount; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            for (unsigned int k = 0; k < GL_POLYGON_VERTEX_SIZE; ++k)
            {
                vertices.push_back(0.0f);
            }
        }
    }
}