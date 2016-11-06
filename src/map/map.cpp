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

void Map::AddPolygon(PMSPolygon polygon)
{
    m_polygons.push_back(polygon);
    ++m_polygonsCount;
}

void Map::EditPolygonVertex(unsigned int polygonIndex, unsigned int vertexIndex, PMSVertex vertex)
{
    m_polygons[polygonIndex].vertices[vertexIndex] = vertex;
}

void Map::SaveMapAsPMS(wxString destinationPath)
{
    wxFile file;
    if (!file.Open(destinationPath, wxFile::write))
    {
        throw wxT("Could not open output file");
    }

    char filler[64];
    memset(filler, 0, sizeof(filler));

    file.Write(&m_version, sizeof(m_version));

    file.Write(&m_descriptionLength, sizeof(m_descriptionLength));
    file.Write(m_description, m_descriptionLength * sizeof(char));
    file.Write(filler, (DESCRIPTION_MAX_LENGTH - m_descriptionLength) * sizeof(char));

    file.Write(&m_textureNameLength, sizeof(m_textureNameLength));
    file.Write(m_textureName, m_textureNameLength * sizeof(char));
    file.Write(filler, (TEXTURE_NAME_MAX_LENGTH - m_textureNameLength) * sizeof(char));

    file.Write(&m_backgroundTopColor, sizeof(m_backgroundTopColor));
    file.Write(&m_backgroundBottomColor, sizeof(m_backgroundBottomColor));
    file.Write(&m_jetCount, sizeof(m_jetCount));
    file.Write(&m_grenadesCount, sizeof(m_grenadesCount));
    file.Write(&m_medikitsCount, sizeof(m_medikitsCount));
    file.Write(&m_weatherType, sizeof(m_weatherType));
    file.Write(&m_stepType, sizeof(m_stepType));
    file.Write(&m_randomId, sizeof(m_randomId));

    file.Write(&m_polygonsCount, sizeof(m_polygonsCount));
    unsigned int i, j;
    for (i = 0; i < m_polygonsCount; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            m_polygons[i].vertices[j].x -= m_centerX;
            m_polygons[i].vertices[j].y -= m_centerY;
            file.Write(&m_polygons[i].vertices[j], sizeof(PMSVertex));
        }
        for (j = 0; j < 3; ++j)
        {
            file.Write(&m_polygons[i].perpendiculars[j], sizeof(PMSVector));
        }
        file.Write(&m_polygons[i].polygonType, sizeof(m_polygons[i].polygonType));
    }

    GenerateSectors();

    file.Write(&m_sectorsSize, sizeof(m_sectorsSize));
    /**
    * In VB6/Pascal, an array can have negative indexes. Basically, Soldat creates an
    * array like this: [-sectorsCount...sectorsCount, -sectorsCount...sectorsCount].
    * This is a 2-dimensional array equal to C++'s [sectorsCount * 2 + 1][sectorsCount * 2 + 1].
    */
    int soldatSectorsCount = m_sectorsCount / 2;
    file.Write(&soldatSectorsCount, sizeof(soldatSectorsCount));
    for (int x = 0; x < m_sectorsCount; ++x)
    {
        for (int y = 0; y < m_sectorsCount; ++y)
        {
            file.Write(&m_sectors[x][y].polygonsCount, sizeof(m_sectors[x][y].polygonsCount));
            for (i = 0; i < m_sectors[x][y].polygonsCount; ++i)
            {
                file.Write(&m_sectors[x][y].polygons[i], sizeof(m_sectors[x][y].polygons[i]));
            }
        }
    }

    file.Write(&m_sceneryInstancesCount, sizeof(m_sceneryInstancesCount));
    for (i = 0; i < m_sceneryInstancesCount; ++i)
    {
        m_sceneryInstances[i].x -= m_centerX;
        m_sceneryInstances[i].y -= m_centerY;
        file.Write(&m_sceneryInstances[i], sizeof(m_sceneryInstances[i]));
    }

    file.Write(&m_sceneryTypesCount, sizeof(m_sceneryTypesCount));
    for (i = 0; i < m_sceneryTypesCount; ++i)
    {
        file.Write(&m_sceneryTypes[i].nameLength, sizeof(m_sceneryTypes[i].nameLength));
        file.Write(m_sceneryTypes[i].name, m_sceneryTypes[i].nameLength * sizeof(char));
        file.Write(filler, (SCENERY_NAME_MAX_LENGTH - m_sceneryTypes[i].nameLength) * sizeof(char));
        file.Write(&m_sceneryTypes[i].timestamp, sizeof(m_sceneryTypes[i].timestamp));
    }

    file.Write(&m_collidersCount, sizeof(m_collidersCount));
    for (i = 0; i < m_collidersCount; ++i)
    {
        m_colliders[i].x -= m_centerX;
        m_colliders[i].y -= m_centerY;
        file.Write(&m_colliders[i], sizeof(m_colliders[i]));
    }

    file.Write(&m_spawnPointsCount, sizeof(m_spawnPointsCount));
    for (i = 0; i < m_spawnPointsCount; ++i)
    {
        m_spawnPoints[i].x -= m_centerX;
        m_spawnPoints[i].y -= m_centerY;
        file.Write(&m_spawnPoints[i], sizeof(m_spawnPoints[i]));
    }

    file.Write(&m_wayPointsCount, sizeof(m_wayPointsCount));
    for (i = 0; i < m_wayPointsCount; ++i)
    {
        m_wayPoints[i].x -= m_centerX;
        m_wayPoints[i].y -= m_centerY;
        file.Write(&m_wayPoints[i], sizeof(m_wayPoints[i]));
    }

    file.Close();
}

void Map::GenerateSectors()
{
    m_sectorsCount = SECTORS_COUNT;

    if (m_width > m_height)
    {
        m_sectorsSize = floor((m_width + 2.0 * MAP_BOUNDARY) / (float)(m_sectorsCount - 1));
    }
    else
    {
        m_sectorsSize = floor((m_height + 2.0 * MAP_BOUNDARY) / (float)(m_sectorsCount - 1));
    }

    for (int x = 0; x < m_sectorsCount; ++x)
    {
        for (int y = 0; y < m_sectorsCount; ++y)
        {
            m_sectors[x][y].polygonsCount = 0;
            m_sectors[x][y].polygons.clear();

            for (int i = 0; i < m_polygonsCount; ++i)
            {
                if (IsPolygonInSector(i,
                                      floor(m_sectorsSize * (x - floor(m_sectorsCount / 2) - 0.5) - 1),
                                      floor(m_sectorsSize * (y - floor(m_sectorsCount / 2) - 0.5) - 1),
                                      m_sectorsSize + 2))
                {
                    // TODO: add polygons' count limit.
                    ++m_sectors[x][y].polygonsCount;
                    m_sectors[x][y].polygons.push_back(i + 1);
                }
            }
        }
    }
}

bool Map::IsPolygonInSector(unsigned short polygonIndex, float sectorX, float sectorY, float sectorSize)
{
    if ((m_polygons[polygonIndex].vertices[0].x < sectorX &&
         m_polygons[polygonIndex].vertices[1].x < sectorX &&
         m_polygons[polygonIndex].vertices[2].x < sectorX) ||
        (m_polygons[polygonIndex].vertices[0].x > sectorX + sectorSize &&
         m_polygons[polygonIndex].vertices[1].x > sectorX + sectorSize &&
         m_polygons[polygonIndex].vertices[2].x > sectorX + sectorSize) ||
        (m_polygons[polygonIndex].vertices[0].y < sectorY &&
         m_polygons[polygonIndex].vertices[1].y < sectorY &&
         m_polygons[polygonIndex].vertices[2].y < sectorY) ||
        (m_polygons[polygonIndex].vertices[0].y > sectorY + sectorSize &&
         m_polygons[polygonIndex].vertices[1].y > sectorY + sectorSize &&
         m_polygons[polygonIndex].vertices[2].y > sectorY + sectorSize))
    {
        return false;
    }

    // Check if any of the polygon's vertices is inside the sector.
    unsigned int i;
    for (i = 0; i < 3; ++i)
    {
        if (m_polygons[polygonIndex].vertices[i].x >= sectorX &&
            m_polygons[polygonIndex].vertices[i].x <= sectorX + sectorSize &&
            m_polygons[polygonIndex].vertices[i].y >= sectorY &&
            m_polygons[polygonIndex].vertices[i].y <= sectorY + sectorSize)
        {
            return true;
        }
    }

    // Check if any of the 4 sector's corners is inside the polygon.
    if (m_polygons[polygonIndex].Contains(sectorX, sectorY) ||
        m_polygons[polygonIndex].Contains(sectorX + sectorSize, sectorY) ||
        m_polygons[polygonIndex].Contains(sectorX + sectorSize, sectorY + sectorSize) ||
        m_polygons[polygonIndex].Contains(sectorX, sectorY + sectorSize))
    {
        return true;
    }

    /**
    * Check intersections between polygon's sides and sector's sides.
    * AB is polygon's side, CD is sector's side.
    */
    unsigned int j;
    float aX, aY, bX, bY, cX, cY, dX, dY;
    for (i = 0; i < 3; ++i)
    {
        j = i + 1;
        if (j > 2)
        {
            j = 0;
        }

        aX = m_polygons[polygonIndex].vertices[i].x;
        aY = m_polygons[polygonIndex].vertices[i].y;
        bX = m_polygons[polygonIndex].vertices[j].x;
        bY = m_polygons[polygonIndex].vertices[j].y;

        // Top side of sector.
        cX = sectorX, cY = sectorY;
        dX = sectorX + sectorSize, dY = sectorY;
        if (Utils::SegmentsIntersect(aX, aY, bX, bY, cX, cY, dX, dY))
        {
            return true;
        }

        // Right side of sector.
        cX = sectorX + sectorSize, cY = sectorY;
        dX = sectorX + sectorSize, dY = sectorY + sectorSize;
        if (Utils::SegmentsIntersect(aX, aY, bX, bY, cX, cY, dX, dY))
        {
            return true;
        }

        // Bottom side of sector.
        cX = sectorX, cY = sectorY + sectorSize;
        dX = sectorX + sectorSize, dY = sectorY + sectorSize;
        if (Utils::SegmentsIntersect(aX, aY, bX, bY, cX, cY, dX, dY))
        {
            return true;
        }

        // Left side of sector.
        cX = sectorX, cY = sectorY;
        dX = sectorX, dY = sectorY + sectorSize;
        if (Utils::SegmentsIntersect(aX, aY, bX, bY, cX, cY, dX, dY))
        {
            return true;
        }
    }

    return false;
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

    m_polygonsMinX = 0.0f;
    m_polygonsMaxX = 0.0f;
    m_polygonsMinY = 0.0f;
    m_polygonsMaxY = 0.0f;

    file.Read(static_cast<void*>(&m_version), sizeof(int));

    file.Read(static_cast<void*>(&m_descriptionLength), sizeof(unsigned char));
    memset(m_description, 0, sizeof(m_description));
    file.Read(static_cast<void*>(m_description), sizeof(char) * m_descriptionLength);
    file.Read(static_cast<void*>(filler), sizeof(char) * (DESCRIPTION_MAX_LENGTH - m_descriptionLength));
    
    file.Read(static_cast<void*>(&m_textureNameLength), sizeof(unsigned char));
    memset(m_textureName, 0, sizeof(m_textureName));
    file.Read(static_cast<void*>(m_textureName), sizeof(char) * m_textureNameLength);
    file.Read(static_cast<void*>(filler), sizeof(char) * (TEXTURE_NAME_MAX_LENGTH - m_textureNameLength));

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
            {
                m_polygonsMinX = tmp.vertices[j].x;
            }
            if (tmp.vertices[j].x > m_polygonsMaxX)
            {
                m_polygonsMaxX = tmp.vertices[j].x;
            }

            if (tmp.vertices[j].y < m_polygonsMinY)
            {
                m_polygonsMinY = tmp.vertices[j].y;
            }
            if (tmp.vertices[j].y > m_polygonsMaxY)
            {
                m_polygonsMaxY = tmp.vertices[j].y;
            }
        }
        for (j = 0; j < 3; ++j)
        {
            file.Read(static_cast<void*>(&tmp.perpendiculars[j]), sizeof(PMSVector));
        }
        file.Read(static_cast<void*>(&tmp.polygonType), sizeof(PMSPolygonType));
        m_polygons.push_back(tmp);
    }

    file.Read(static_cast<void*>(&m_sectorsSize), sizeof(int));
    file.Read(static_cast<void*>(&m_sectorsCount), sizeof(int));
    for (int x = 0; x <= m_sectorsCount * 2; ++x)
    {
        for (int y = 0; y <= m_sectorsCount * 2; ++y)
        {
            file.Read(static_cast<void*>(&m_sectors[x][y].polygonsCount), sizeof(unsigned short));

            for (i = 0; i < m_sectors[x][y].polygonsCount; ++i)
            {
                unsigned short tmpPolygonIndex;
                file.Read(static_cast<void*>(&tmpPolygonIndex), sizeof(unsigned short));
                m_sectors[x][y].polygons.push_back(tmpPolygonIndex);
            }
        }
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
        file.Read(static_cast<void*>(tmp.name), sizeof(char) * tmp.nameLength);
        file.Read(static_cast<void*>(filler), sizeof(char) * (SCENERY_NAME_MAX_LENGTH - tmp.nameLength));
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

    file.Read(static_cast<void*>(&m_wayPointsCount), sizeof(m_wayPointsCount));
    for (i = 0; i < m_wayPointsCount; ++i)
    {
        PMSWayPoint tmp;

        file.Read(static_cast<void*>(&tmp), sizeof(PMSWayPoint));
        m_wayPoints.push_back(tmp);
    }

    m_width = fabs(m_polygonsMaxX - m_polygonsMinX);
    m_height = fabs(m_polygonsMaxY - m_polygonsMinY);

    m_centerX = floor((m_polygonsMinX + m_polygonsMaxX) / 2.0);
    m_centerY = floor((m_polygonsMinY + m_polygonsMaxY) / 2.0);

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
        m_boundariesXY[LEFT_BOUNDARY] -= (m_height - m_width) / 2.0;
        m_boundariesXY[RIGHT_BOUNDARY] += (m_height - m_width) / 2.0;
    }
    else
    {
        m_boundariesXY[TOP_BOUNDARY] -= (m_width - m_height) / 2.0;
        m_boundariesXY[BOTTOM_BOUNDARY] += (m_width - m_height) / 2.0;
    }

    m_boundariesXY[TOP_BOUNDARY] -= MAP_BOUNDARY;
    m_boundariesXY[BOTTOM_BOUNDARY] += MAP_BOUNDARY;
    m_boundariesXY[LEFT_BOUNDARY] -= MAP_BOUNDARY;
    m_boundariesXY[RIGHT_BOUNDARY] += MAP_BOUNDARY;
}
