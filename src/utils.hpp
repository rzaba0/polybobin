#ifndef UTILS_HPP
#define UTILS_HPP

enum PMSPolygonType : unsigned char;
struct PMSColor;

namespace Utils
{
    PMSColor GetPolygonColorByType(PMSPolygonType polygonType);

    // http://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
    float Sign(float aX, float aY, float bX, float bY, float cX, float cY);
}

#endif