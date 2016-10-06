#include "glpolygons.hpp"
#include "image.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLPolygons::Render(glm::mat4 transform)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
    glBindVertexArray(0);
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
    int polygonsCount = polygons.size(), i, j;

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
    m_verticesCount = polygonsCount*3;

    //GLuint vbo;
    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
        if (vertices.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, GL_POLYGON_VERTEX_SIZE*m_verticesCount, &vertices[0], GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_POLYGON_VERTEX_SIZE, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_POLYGON_VERTEX_SIZE, (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, GL_POLYGON_VERTEX_SIZE, (GLvoid*)(7*sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}