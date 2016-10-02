#include "map.hpp"
#include <wx/file.h>

Map::Map(wxString path)
{
    if (path.IsEmpty())
    {
        LoadDefaultMap();
    }
    else
    {
        LoadMap(path);
    }
}

void Map::LoadDefaultMap()
{
    m_descriptionLength = 20;
    strcpy(m_description, "Made using Polybobin");

    m_textureNameLength = 10;
    strcpy(m_textureName, "banana.png");

    m_backgroundTopColor = PMSColor(100, 200, 100, 255);
    m_backgroundBottomColor = PMSColor(50, 50, 50, 255);

    m_jetCount = 255;
    m_grenadesCount = 3;
    m_medikitsCount = 3;
    m_weatherType = wtNONE;
    m_stepType = stHARD_GROUND;
    m_randomId = 12345;

    m_polygonsCount = 0;
    m_sceneryInstances = 0;
    m_sceneryTypesCount = 0;
    m_collidersCount = 0;
    m_spawnPointsCount = 0;
    m_wayPointsCount = 0;

    m_polygonsMinX = -MAP_BOUNDARY;
    m_polygonsMaxX = MAP_BOUNDARY;
    m_polygonsMinY = -MAP_BOUNDARY;
    m_polygonsMaxY = MAP_BOUNDARY;

    m_width = fabs(m_polygonsMaxX - m_polygonsMinX);
    m_height = fabs(m_polygonsMaxY - m_polygonsMinY);

    UpdateBoundaries();
}

void Map::LoadMap(wxString mapPath)
{
    m_isPathSet = true;

    wxFile file;
    if (!file.Open(mapPath))
    {
        throw wxString("Could not open map ") + mapPath;
    }

    int i, j;
    char filler[64];

    m_boundariesXY[TOP_BOUNDARY] = -MAP_BOUNDARY;
    m_boundariesXY[BOTTOM_BOUNDARY] = MAP_BOUNDARY;
    m_boundariesXY[LEFT_BOUNDARY] = -MAP_BOUNDARY;
    m_boundariesXY[RIGHT_BOUNDARY] = MAP_BOUNDARY;

    m_polygonsMinX = -MAP_BOUNDARY;
    m_polygonsMaxX = MAP_BOUNDARY;
    m_polygonsMinY = -MAP_BOUNDARY;
    m_polygonsMaxY = MAP_BOUNDARY;

    file.Read(static_cast<void*>(&m_version), sizeof(int));
    file.Read(static_cast<void*>(&m_descriptionLength), sizeof(unsigned char));

    memset(m_description, 0, sizeof(m_description));
    file.Read(static_cast<void*>(m_description), sizeof(char)*m_descriptionLength);
    file.Read(static_cast<void*>(filler), sizeof(char)*(MAX_LENGTH_MAP_DESCRIPTION-m_descriptionLength));
    
    file.Read(static_cast<void*>(&m_textureNameLength), sizeof(unsigned char));
    memset(m_textureName, 0, sizeof(m_textureName));
    file.Read(static_cast<void*>(m_textureName), sizeof(char)*m_textureNameLength);
    file.Read(static_cast<void*>(filler), sizeof(char)*(MAX_LENGTH_MAP_TEXTURE_NAME-m_textureNameLength));

    file.Read(static_cast<void*>(&m_backgroundTopColor), sizeof(PMSColor));
    file.Read(static_cast<void*>(&m_backgroundBottomColor), sizeof(PMSColor));
    file.Read(static_cast<void*>(&m_jetCount), sizeof(int));
    file.Read(static_cast<void*>(&m_grenadesCount), sizeof(unsigned char));
    file.Read(static_cast<void*>(&m_medikitsCount), sizeof(unsigned char));
    file.Read(static_cast<void*>(&m_weatherType), sizeof(PMSWeatherType));
    file.Read(static_cast<void*>(&m_stepType), sizeof(PMSStepType));
    file.Read(static_cast<void*>(&m_randomId), sizeof(int));

    file.Read(static_cast<void*>(&m_polygonsCount), sizeof(int));
    m_polygons.clear();
    for (i = 0; i < m_polygonsCount; ++i)
    {
        PMSPolygon tmp;

        for (j = 0; j < 3; ++j)
        {
            file.Read(static_cast<void*>(&tmp.vertices[j]), sizeof(PMSVertex));

            if (tmp.vertices[j].x < m_polygonsMinX)
                m_polygonsMinX = tmp.vertices[j].x;
            if (tmp.vertices[j].x > m_polygonsMaxX)
                m_polygonsMaxX = tmp.vertices[j].x;

            if (tmp.vertices[j].y < m_polygonsMinY)
                m_polygonsMinY = tmp.vertices[j].y;
            if (tmp.vertices[j].y > m_polygonsMaxY)
                m_polygonsMaxY = tmp.vertices[j].y;
        }
        for (j = 0; j < 3; ++j)
        {
            file.Read(static_cast<void*>(&tmp.perpendiculars[j]), sizeof(PMSVector));
        }
        file.Read(static_cast<void*>(&tmp.polygonType), sizeof(PMSPolygonType));
        m_polygons.push_back(tmp);
    }

    file.Read(static_cast<void*>(&m_sectorsDivision), sizeof(int));
    file.Read(static_cast<void*>(&m_sectorsCount), sizeof(int));
    m_sectors.clear();
    for (i = 0; i < ((m_sectorsCount*2)+1)*((m_sectorsCount*2)+1); ++i)
    {
        PMSSector tmpSector;
        file.Read(static_cast<void*>(&tmpSector.polygonsCount), sizeof(unsigned short));
        for (j = 0; j < tmpSector.polygonsCount; ++j)
        {
            unsigned short tmpPolygonIndex;
            file.Read(static_cast<void*>(&tmpPolygonIndex), sizeof(unsigned short));
            tmpSector.polygons.push_back(tmpPolygonIndex);
        }
        m_sectors.push_back(tmpSector);
    }

    file.Read(static_cast<void*>(&m_sceneryInstancesCount), sizeof(int));
    m_sceneryInstances.clear();
    for (i = 0; i < m_sceneryInstancesCount; ++i)
    {
        PMSScenery tmp;

        file.Read(static_cast<void*>(&tmp), sizeof(PMSScenery));
        m_sceneryInstances.push_back(tmp);
    }

    file.Read(static_cast<void*>(&m_sceneryTypesCount), sizeof(int));
    m_sceneryTypes.clear();
    for (i = 0; i < m_sceneryTypesCount; ++i)
    {
        PMSSceneryType tmp;

        memset(tmp.name, 0, sizeof(tmp.name));
        file.Read(static_cast<void*>(&tmp.nameLength), sizeof(unsigned char));
        file.Read(static_cast<void*>(tmp.name), sizeof(char)*tmp.nameLength);
        file.Read(static_cast<void*>(filler), sizeof(char)*(MAX_LENGTH_MAP_SCENERY_NAME-tmp.nameLength));
        file.Read(static_cast<void*>(&tmp.timestamp), sizeof(PMSTimestamp));

        m_sceneryTypes.push_back(tmp);
    }

    file.Read(static_cast<void*>(&m_collidersCount), sizeof(int));
    m_colliders.clear();
    for (i = 0; i < m_collidersCount; ++i)
    {
        PMSCollider tmp;

        file.Read(static_cast<void*>(&tmp), sizeof(PMSCollider));
        m_colliders.push_back(tmp);
    }

    file.Read(static_cast<void*>(&m_spawnPointsCount), sizeof(int));
    m_spawnPoints.clear();
    for (i = 0; i < m_spawnPointsCount; ++i)
    {
        PMSSpawnPoint tmp;

        file.Read(static_cast<void*>(&tmp), sizeof(PMSSpawnPoint));
        m_spawnPoints.push_back(tmp);
    }

    m_width = fabs(m_polygonsMaxX - m_polygonsMinX);
    m_height = fabs(m_polygonsMaxY - m_polygonsMinY);

    UpdateBoundaries();
    
    file.Close();
}

void Map::UpdateBoundaries()
{
    m_boundariesXY[TOP_BOUNDARY] = m_polygonsMinY;
    m_boundariesXY[BOTTOM_BOUNDARY] = m_polygonsMaxY;
    m_boundariesXY[LEFT_BOUNDARY] = m_polygonsMinX;
    m_boundariesXY[RIGHT_BOUNDARY] = m_polygonsMaxX;

    if (m_height > m_width)
    {
        m_boundariesXY[LEFT_BOUNDARY] -= (m_height-m_width) / 2.0;
        m_boundariesXY[RIGHT_BOUNDARY] += (m_height-m_width) / 2.0;
    }
    else
    {
        m_boundariesXY[TOP_BOUNDARY] -= (m_width-m_height) / 2.0;
        m_boundariesXY[BOTTOM_BOUNDARY] += (m_width-m_height) / 2.0;
    }

    m_boundariesXY[TOP_BOUNDARY] -= MAP_BOUNDARY;
    m_boundariesXY[BOTTOM_BOUNDARY] += MAP_BOUNDARY;
    m_boundariesXY[LEFT_BOUNDARY] -= MAP_BOUNDARY;
    m_boundariesXY[RIGHT_BOUNDARY] += MAP_BOUNDARY;
}
