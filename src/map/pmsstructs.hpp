#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "pmsconstants.hpp"
#include "pmsenums.hpp"
#include "../utils.hpp"

struct DOSTime
{
    unsigned short second : 5, minute : 6, hour : 5;
};

struct DOSDate
{
    unsigned short day : 5, month : 4, year : 7;
};

struct PMSCollider
{
    int active;
    float x, y, radius;
};

struct PMSColor
{
    std::uint8_t blue, green, red, alpha;
    PMSColor();
    PMSColor(wxColor col);
    PMSColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);
};

struct PMSVector
{
    float x, y, z;
};

struct PMSVertex
{
    float x, y, z;
    // Those are most likely texture-related
    float rhw;
    PMSColor color;
    float textureS, textureT; // S corresponds to X axis, T corresponds to Y axis.
};

struct PMSPolygon
{
    PMSVertex vertices[3];
    PMSVector perpendiculars[3];
    PMSPolygonType polygonType;

    /**
     * \brief Checks if the vertices are arranged in clock-wise order.
     * http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order
     */
    bool AreVerticesClockwise() const;

    /**
     * \brief Checks if the point at given coordinates is inside the polygon.
     */
    bool Contains(float x, float y) const;
};

struct PMSScenery
{
    bool active;
    unsigned short style;
    int width,
        height;
    float x, y, rotation, scaleX, scaleY;
    int alpha;
    PMSColor color;
    int level;

    /**
     * \brief Checks if the point at given coordinates is inside the scenery.
     */
    bool Contains(float x, float y) const;
};

struct PMSTimestamp
{
    DOSTime time;
    DOSDate date;
};

struct PMSSceneryType
{
    unsigned char nameLength;
    char name[SCENERY_NAME_MAX_LENGTH];
    PMSTimestamp timestamp;
};

struct PMSSector
{
    unsigned short polygonsCount;
    wxVector<unsigned short> polygons;
};

struct PMSSpawnPoint
{
    int active, x, y;
    PMSSpawnPointType type;
};

struct PMSWayPoint
{
    bool active;
    unsigned char filler1[3];
    int id;
    int x, y;
    bool left, right, up, down, jet;
    unsigned char path;
    PMSSpecialAction specialAction;
    unsigned char c2, c3; //?
    unsigned char filler2[3];
    int connectionsCount;
    int connections[20];
};