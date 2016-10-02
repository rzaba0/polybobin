#include "glbackground.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLBackground::Render(glm::mat4 transform)
{
    m_shaderProgram.Use();
    
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLBackground::SetupShaderProgram()
{
    const GLchar *vertexShaderSource =
    {
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec4 color;\n"
        "uniform mat4 transform;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = transform*(vec4(position, 1.0f));\n"
        "   vertexColor = color;\n"
        "}\n"
    };

    const GLchar *fragmentShaderSource =
    {
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vertexColor;\n"
        "}\n"
    };

    SetupShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void GLBackground::SetupVAO(PMSColor backgroundTopColor, PMSColor backgroundBottomColor,
                            float *backgroundRectangleBoundaries)
{
    int i;
    GLfloat vertices[4][7];

    for (i = 0; i < 4; ++i)
        vertices[i][2] = 1.0f;

    for (i = 0; i < 2; ++i)
    {
        vertices[i][1] = backgroundRectangleBoundaries[Map::TOP_BOUNDARY];
        vertices[i][3] = backgroundTopColor.red / 255.0f,
        vertices[i][4] = backgroundTopColor.green / 255.0f,
        vertices[i][5] = backgroundTopColor.blue / 255.0f,
        vertices[i][6] = backgroundTopColor.alpha / 255.0f;
    }

    for (i = 2; i < 4; ++i)
    {
        vertices[i][1] = backgroundRectangleBoundaries[Map::BOTTOM_BOUNDARY];
        vertices[i][3] = backgroundBottomColor.red / 255.0f,
        vertices[i][4] = backgroundBottomColor.green / 255.0f,
        vertices[i][5] = backgroundBottomColor.blue / 255.0f,
        vertices[i][6] = backgroundBottomColor.alpha / 255.0f;
    }

    vertices[0][0] = backgroundRectangleBoundaries[Map::LEFT_BOUNDARY],
    vertices[1][0] = backgroundRectangleBoundaries[Map::RIGHT_BOUNDARY],
    vertices[2][0] = backgroundRectangleBoundaries[Map::LEFT_BOUNDARY],
    vertices[3][0] = backgroundRectangleBoundaries[Map::RIGHT_BOUNDARY];

    GLuint indices[] =
    {
        0, 1, 2,
        1, 3, 2
    };

    GLuint vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}