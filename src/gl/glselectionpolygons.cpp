#include "glselectionpolygons.hpp"
#include "../constants.hpp"
#include "image.hpp"
#include "../utils.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLSelectionPolygons::RenderSelected(glm::mat4 transform, wxVector<unsigned int> selectedPolygonsIds)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    for (unsigned int i = 0; i < selectedPolygonsIds.size(); ++i)
    {
        glDrawArrays(GL_TRIANGLES, selectedPolygonsIds[i] * 3, 3);
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
    unsigned int i, j;
    for (i = 0; i < polygons.size(); ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            vertices.push_back(polygons[i].vertices[j].x);
            vertices.push_back(polygons[i].vertices[j].y);
            vertices.push_back(polygons[i].vertices[j].z);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(.5f);
            vertices.push_back(0.0f); //These values will be set manually later.
            vertices.push_back(0.0f);
        }

        AdjustTextureCoordinates(vertices, i * 3 * 9);
        AdjustVertexColor(vertices, i * 3 * 9, polygons[i].polygonType);
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, GL_SELECTION_POLYGON_VERTEX_SIZE * 3 * polygons.size(), &vertices[0], GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_SELECTION_POLYGON_VERTEX_SIZE, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_SELECTION_POLYGON_VERTEX_SIZE, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, GL_SELECTION_POLYGON_VERTEX_SIZE, (GLvoid*)(7 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void GLSelectionPolygons::AdjustTextureCoordinates(wxVector<GLfloat> &vertices, int index)
{
    GLfloat polygonX[3], polygonY[3], textureX, textureY;
    polygonX[0] = vertices[index], polygonY[0] = vertices[index+1];
    polygonX[1] = vertices[index+ELEMENTS_PER_VERTEX], polygonY[1] = vertices[index+ELEMENTS_PER_VERTEX+1];
    polygonX[2] = vertices[index+2*ELEMENTS_PER_VERTEX], polygonY[2] = vertices[index+2*ELEMENTS_PER_VERTEX+1];

    const int INDEX_TEXTURE_X = 7, INDEX_TEXTURE_Y = 8;

    vertices[index+INDEX_TEXTURE_X] = 0.0f, vertices[index+INDEX_TEXTURE_Y] = 0.0f;
    textureX = (polygonX[1]-polygonX[0]) / (GLfloat) m_textureWidth,
    textureY = (polygonY[1]-polygonY[0]) / (GLfloat) m_textureHeight;
    vertices[index+ELEMENTS_PER_VERTEX+INDEX_TEXTURE_X] = textureX,
    vertices[index+ELEMENTS_PER_VERTEX+INDEX_TEXTURE_Y] = textureY;

    textureX = (polygonX[2]-polygonX[0])/(GLfloat) m_textureWidth,
    textureY = (polygonY[2]-polygonY[0])/(GLfloat) m_textureHeight;
    vertices[index+2*ELEMENTS_PER_VERTEX+INDEX_TEXTURE_X] = textureX,
    vertices[index+2*ELEMENTS_PER_VERTEX+INDEX_TEXTURE_Y] = textureY;
}

void GLSelectionPolygons::AdjustVertexColor(wxVector<GLfloat> &vertices, int index, PMSPolygonType polygonType)
{
    PMSColor color = Utils::GetPolygonColorByType(polygonType);

    const int INDEX_COLOR_R = 3,
              INDEX_COLOR_G = 4,
              INDEX_COLOR_B = 5;

    for (int i = 0; i < 3; ++i)
    {
        vertices[index + i * ELEMENTS_PER_VERTEX + INDEX_COLOR_R] = (GLfloat) color.red / 255.0f,
        vertices[index + i * ELEMENTS_PER_VERTEX + INDEX_COLOR_G] = (GLfloat) color.green / 255.0f,
        vertices[index + i * ELEMENTS_PER_VERTEX + INDEX_COLOR_B] = (GLfloat) color.blue / 255.0f;
    }
}
