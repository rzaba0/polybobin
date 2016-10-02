#include "map/pmsenums.hpp"
#include "map/pmsstructs.hpp"
#include "utils.hpp"

namespace Utils
{
    PMSColor GetPolygonColorByType(PMSPolygonType polygonType)
    {
        switch (polygonType)
        {
            case ptNORMAL:
                return PMSColor(255, 255, 255, 255);

            case ptONLY_BULLETS_COLLIDE:
                return PMSColor(0, 0, 0, 127);

            case ptONLY_PLAYERS_COLLIDE:
                return PMSColor(255, 255, 255, 127);

            case ptNO_COLLIDE:
                return PMSColor(0, 255, 0, 150);

            case ptICE:
                return PMSColor(0, 255, 255, 255);

            case ptDEADLY:
                return PMSColor(255, 0, 0, 255);

            case ptBLOODY_DEADLY:
                return PMSColor(255, 0, 0, 255);

            case ptHURTS:
                return PMSColor(255, 255, 0, 255);

            case ptREGENERATES:
                return PMSColor(0, 0, 255, 255);

            case ptLAVA:
                return PMSColor(255, 127, 0, 255);

            // TODO: handle other polygon types.

            case ptBOUNCY:
                return PMSColor(255, 0, 255, 255);

            case ptEXPLOSIVE:
                return PMSColor(255, 20, 20, 255);

            default:
                return PMSColor(127, 127, 127, 255);
        }
    }

    float Sign(float aX, float aY, float bX, float bY, float cX, float cY)
    {
        return (aX-cX) * (bY-cY) - (bX-cX) * (aY-cY);
    }
}