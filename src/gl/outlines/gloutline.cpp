#include "gloutline.hpp"

void GLOutline::SetupShaderProgram()
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