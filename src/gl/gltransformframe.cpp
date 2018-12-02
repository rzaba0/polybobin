#include "gltransformframe.hpp"
#include "../utils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "../constants.hpp"

void GLTransformFrame::Move(const wxPoint bottomLeft,
          const wxPoint bottomRight,
          const wxPoint topLeft,
          const wxPoint topRight)
{
    if (m_bottomLeft != bottomLeft ||
        m_bottomRight != bottomRight ||
        m_topLeft != topLeft ||
        m_topRight != m_topRight)
    {
        m_bottomLeft = bottomLeft;
        m_bottomRight = bottomRight;
        m_topLeft = topLeft;
        m_topRight = topRight;

        wxVector<GLfloat> vertices;

        GenerateGLBufferVertices(vertices);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, GL_FRAME_BYTES * 4 + GL_ORIGIN_BYTES * 4, &vertices[0]);
    }
}

void GLTransformFrame::Render(const glm::mat4& transform, const glm::mat4& translation)
{
    m_shaderProgram.Use();
    glUniformMatrix4fv(m_shaderProgram.GetUniformLocation("transform"),
                       1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(m_vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // frame 4 lines
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // 4 origins, 2 triangles each
    glDrawArrays(GL_TRIANGLES, 4, 4 * 3 * 2);
    glBindVertexArray(0);
}

void GLTransformFrame::SetupVAO()
{
    m_bottomLeft = wxPoint(0.0, 0.0);
    m_bottomRight = wxPoint(100.0, 0.0);
    m_topLeft = wxPoint(0.0, 100.0);
    m_topRight = wxPoint(100.0, 100.0);

    wxVector<GLfloat> vertices;
    GenerateGLBufferVertices(vertices);

    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
    if (vertices.size() > 0)
    {

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, GL_FRAME_BYTES * 4 + GL_ORIGIN_BYTES * 4, &vertices[0], GL_DYNAMIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_FRAME_BYTES, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_FRAME_BYTES, (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void GLTransformFrame::SetupShaderProgram()
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

void GLTransformFrame::GenerateGLBufferVertices(wxVector<GLfloat> &vertices)
{
    unsigned int i;

    wxColor color = TRANSFORM_TOOL_FRAME_COLOR;

    vertices.push_back((GLfloat)m_bottomLeft.x);
    vertices.push_back((GLfloat)m_bottomLeft.y);
    vertices.push_back((GLfloat)1.0);
    vertices.push_back((GLfloat)color.Red() / 255.0f);
    vertices.push_back((GLfloat)color.Green() / 255.0f);
    vertices.push_back((GLfloat)color.Blue() / 255.0f);
    vertices.push_back((GLfloat)color.Alpha() / 255.0f);

    vertices.push_back((GLfloat)m_bottomRight.x);
    vertices.push_back((GLfloat)m_bottomRight.y);
    vertices.push_back((GLfloat)1.0);
    vertices.push_back((GLfloat)color.Red() / 255.0f);
    vertices.push_back((GLfloat)color.Green() / 255.0f);
    vertices.push_back((GLfloat)color.Blue() / 255.0f);
    vertices.push_back((GLfloat)color.Alpha() / 255.0f);

    vertices.push_back((GLfloat)m_topRight.x);
    vertices.push_back((GLfloat)m_topRight.y);
    vertices.push_back((GLfloat)1.0);
    vertices.push_back((GLfloat)color.Red() / 255.0f);
    vertices.push_back((GLfloat)color.Green() / 255.0f);
    vertices.push_back((GLfloat)color.Blue() / 255.0f);
    vertices.push_back((GLfloat)color.Alpha() / 255.0f);

    vertices.push_back((GLfloat)m_topLeft.x);
    vertices.push_back((GLfloat)m_topLeft.y);
    vertices.push_back((GLfloat)1.0);
    vertices.push_back((GLfloat)color.Red() / 255.0f);
    vertices.push_back((GLfloat)color.Green() / 255.0f);
    vertices.push_back((GLfloat)color.Blue() / 255.0f);
    vertices.push_back((GLfloat)color.Alpha() / 255.0f);

    for (unsigned i = 0; i < 4; i++)
    {
        vertices.push_back((GLfloat)vertices[7 * i] - TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)vertices[7 * i + 1] - TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)1.0);
        vertices.push_back((GLfloat)color.Red() / 255.0f);
        vertices.push_back((GLfloat)color.Green() / 255.0f);
        vertices.push_back((GLfloat)color.Blue() / 255.0f);
        vertices.push_back((GLfloat)color.Alpha() / 255.0f);

        vertices.push_back((GLfloat)vertices[7 * i] - TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)vertices[7 * i + 1] + TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)1.0);
        vertices.push_back((GLfloat)color.Red() / 255.0f);
        vertices.push_back((GLfloat)color.Green() / 255.0f);
        vertices.push_back((GLfloat)color.Blue() / 255.0f);
        vertices.push_back((GLfloat)color.Alpha() / 255.0f);
        
        vertices.push_back((GLfloat)vertices[7 * i] + TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)vertices[7 * i + 1] - TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)1.0);
        vertices.push_back((GLfloat)color.Red() / 255.0f);
        vertices.push_back((GLfloat)color.Green() / 255.0f);
        vertices.push_back((GLfloat)color.Blue() / 255.0f);
        vertices.push_back((GLfloat)color.Alpha() / 255.0f);

        vertices.push_back((GLfloat)vertices[7 * i] + TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)vertices[7 * i + 1] + TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)1.0);
        vertices.push_back((GLfloat)color.Red() / 255.0f);
        vertices.push_back((GLfloat)color.Green() / 255.0f);
        vertices.push_back((GLfloat)color.Blue() / 255.0f);
        vertices.push_back((GLfloat)color.Alpha() / 255.0f);

        vertices.push_back((GLfloat)vertices[7 * i] - TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)vertices[7 * i + 1] + TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)1.0);
        vertices.push_back((GLfloat)color.Red() / 255.0f);
        vertices.push_back((GLfloat)color.Green() / 255.0f);
        vertices.push_back((GLfloat)color.Blue() / 255.0f);
        vertices.push_back((GLfloat)color.Alpha() / 255.0f);
        
        vertices.push_back((GLfloat)vertices[7 * i] + TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)vertices[7 * i + 1] - TRANSFORM_TOOL_ORIGIN_SIZE / 2);
        vertices.push_back((GLfloat)1.0);
        vertices.push_back((GLfloat)color.Red() / 255.0f);
        vertices.push_back((GLfloat)color.Green() / 255.0f);
        vertices.push_back((GLfloat)color.Blue() / 255.0f);
        vertices.push_back((GLfloat)color.Alpha() / 255.0f);
    }
}

