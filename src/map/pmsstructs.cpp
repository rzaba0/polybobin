#include "pmsstructs.hpp"

PMSColor::PMSColor()
    : blue{255}, green{255}, red{255}, alpha{255}
{
}

PMSColor::PMSColor(wxColor col)
    :  blue{col.Blue()}, green{col.Green()}, red{col.Red()}, alpha{col.Alpha()}
{
}

PMSColor::PMSColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
    :  blue{b}, green{g}, red{r}, alpha{a}
{
}

bool PMSScenery::Contains(float x, float y) const
{
    float rectX[4], rectY[4];

    rectX[0] = 0.0, rectY[0] = 0.0;
    rectX[1] = width * scaleX, rectY[1] = 0.0;
    rectX[2] = width * scaleX, rectY[2] = height * scaleY;
    rectX[3] = 0.0, rectY[3] = height * scaleY;

    float rotatedX, rotatedY;
    unsigned int i;
    for (i = 0; i < 4; ++i)
    {
        rotatedX = rectX[i] * cos(-rotation) - rectY[i] * sin(-rotation);
        rotatedY = rectX[i] * sin(-rotation) + rectY[i] * cos(-rotation);
        rectX[i] = rotatedX + this->x;
        rectY[i] = rotatedY + this->y;
    }

    bool b[4];
    for (i = 0; i < 3; ++i)
    {
        b[i] = Utils::Sign(x, y, rectX[i], rectY[i], rectX[i + 1], rectY[i + 1]) < 0.0f;
    }
    b[3] = Utils::Sign(x, y, rectX[3], rectY[3], rectX[0], rectY[0]) < 0.0f;

    return (b[0] == b[1] && b[1] == b[2] && b[2] == b[3]);
}

bool PMSPolygon::AreVerticesClockwise() const
{
    float sum = 0.0f;

    for (unsigned int i = 0; i < 3; ++i)
    {
        unsigned int j = i + 1;
        if (j > 2)
        {
            j = 0;
        }
        sum += (vertices[j].x - vertices[i].x) * (vertices[j].y + vertices[i].y);
    }

    return sum < 0.0f;
}

bool PMSPolygon::Contains(float x, float y) const
{
    bool b1, b2, b3;

    b1 = Utils::Sign(x, y,
        vertices[0].x, vertices[0].y,
        vertices[1].x, vertices[1].y) < 0.0f;

    b2 = Utils::Sign(x, y,
        vertices[1].x, vertices[1].y,
        vertices[2].x, vertices[2].y) < 0.0f;

    b3 = Utils::Sign(x, y,
        vertices[2].x, vertices[2].y,
        vertices[0].x, vertices[0].y) < 0.0f;

    return (b1 == b2 && b2 == b3);
}

// TODO: consider setting active variable too.
PMSSpawnPoint::PMSSpawnPoint()
    : x{0}
    , y{0}
    , type{PMSSpawnPointType::sptALPHA}
{
}

PMSSpawnPoint::PMSSpawnPoint(bool active, int x, int y, PMSSpawnPointType type)
    : active{active}
    , x{x}
    , y{y}
    , type{type}
{
}
