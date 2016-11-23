#ifndef MAP_HPP
#define MAP_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "pmsconstants.hpp"
#include "pmsenums.hpp"
#include "pmsstructs.hpp"

/**
 * \brief Represents Soldat map file (.PMS format).
 */
class Map
{
    public:
        Map() {};

        /** \brief If path is empty, it loads default empty map.
         *  If path is not empty, it loads the map from file.
         */
        Map(wxString path);

        void AddPolygon(PMSPolygon polygon);
        void EditPolygonVertex(unsigned int polygonIndex, unsigned int vertexIndex, PMSVertex vertex);

        void EditScenery(unsigned int sceneryIndex, PMSScenery scenery);
        
        void SaveMapAsPMS(wxString destinationPath);

        PMSColor GetBackgroundTopColor()
        {
            return m_backgroundTopColor;
        }

        PMSColor GetBackgroundBottomColor()
        {
            return m_backgroundBottomColor;
        }

        float *GetBoundaries()
        {
            return m_boundariesXY;
        }

        wxVector<PMSPolygon> GetPolygons()
        {
            return m_polygons;
        }

        unsigned int GetPolygonsCount()
        {
            return m_polygons.size();
        }

        wxVector<PMSScenery> GetSceneryInstances()
        {
            return m_sceneryInstances;
        }

        wxVector<PMSSceneryType> GetSceneryTypes()
        {
            return m_sceneryTypes;
        }

        wxVector <PMSSpawnPoint> GetSpawnPoints()
        {
            return m_spawnPoints;
        }

        char *GetTextureName()
        {
            return m_textureName;
        }

        enum MapBoundary
        {
            TOP_BOUNDARY = 0,
            BOTTOM_BOUNDARY,
            LEFT_BOUNDARY,
            RIGHT_BOUNDARY
        };

    private:
        float m_boundariesXY[4];
        bool m_isPathSet;
        float m_polygonsMinX, m_polygonsMaxX,
              m_polygonsMinY, m_polygonsMaxY,
              m_width, m_height,
              m_centerX, m_centerY;

        int m_version;

        unsigned char m_descriptionLength;
        char m_description[DESCRIPTION_MAX_LENGTH];

        unsigned char m_textureNameLength;
        char m_textureName[TEXTURE_NAME_MAX_LENGTH];

        PMSColor m_backgroundTopColor, m_backgroundBottomColor;

        int m_jetCount;
        unsigned char m_grenadesCount, m_medikitsCount;
        PMSWeatherType m_weatherType;
        PMSStepType m_stepType;
        int m_randomId;

        int m_polygonsCount;
        wxVector<PMSPolygon> m_polygons;

        int m_sectorsSize;
        int m_sectorsCount;
        PMSSector m_sectors[SECTORS_COUNT][SECTORS_COUNT];

        int m_sceneryInstancesCount;
        wxVector<PMSScenery> m_sceneryInstances;

        int m_sceneryTypesCount;
        wxVector<PMSSceneryType> m_sceneryTypes;

        int m_collidersCount;
        wxVector<PMSCollider> m_colliders;

        int m_spawnPointsCount;
        wxVector<PMSSpawnPoint> m_spawnPoints;

        int m_wayPointsCount;
        wxVector<PMSWayPoint> m_wayPoints;

        void GenerateSectors();
        // sectorX and sectorY are sector's coordinates, not sector's indexes inside grid.
        bool IsPolygonInSector(unsigned short polygonIndex, float sectorX, float sectorY, float sectorSize);
        void LoadDefaultMap();
        void LoadMap(wxString mapPath);
        void UpdateBoundaries();
};

#endif
