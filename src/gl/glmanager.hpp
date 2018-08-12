#ifndef GLMANAGER_HPP
#define GLMANAGER_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "camera.hpp"
#include "displaysettings.hpp"
#include "../map/map.hpp"
#include "../settings.hpp"

#include "glbackground.hpp"
#include "glpolygons.hpp"
#include "glscenery.hpp"
#include "glspawnpoints.hpp"

#include "outlines/gloutlinepolygons.hpp"
#include "outlines/gloutlinescenery.hpp"

#include "../selection.hpp"
#include "../polygonselection.hpp"
#include "glselectionpolygons.hpp"

/**
 * \brief Handles all OpenGL drawing.
 */
class GLManager
{
    public:
        GLManager(Settings settings, Map &map);

        void AddPolygon(PMSPolygonType polygonType, PMSVertex firstVertex);
        void AddSpawnPoint(PMSSpawnPoint spawnPoint);

        void EditPolygonVertex(unsigned int polygonIndex, PMSPolygonType polygonType,
                               unsigned int vertexIndex, PMSVertex vertex);

        void EditScenery(unsigned int sceneryIndex, PMSScenery scenery);
        void EditSpawnPoint(unsigned int spawnPointIdx, PMSSpawnPoint spawnPoint);

        void ResetSpawnPoints(wxVector<PMSSpawnPoint> spawnPoints);
        void ResetPolygons(wxVector<PMSPolygon> polygons);
        void ResetSceneries(wxVector<PMSScenery> sceneryInstances);

        void ApplyPolygonSelection(const PolygonSelection& selectedPolygons);

        unsigned int GetTextureWidth();
        unsigned int GetTextureHeight();

        void Init();
        bool IsGLReady();
        void Render(Camera camera, wxSize canvasSize, DisplaySettings displaySettings,
                    const PolygonSelection& selectedPolygons, const Selection& selectedScenery,
                    bool addingPolygon);

        void SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor);
        void SetPolygonsTexture(wxString textureFilename);

        void SetupShaders();
        void SetupTextures();
        void SetupVertices();

        void AddSceneryTexture(wxString sceneryName);

        void SetFillPolygonSelection(const PolygonSelection& selectedPolygons, bool mode = true);

    private:
        bool m_glReady;
        Map &m_map;
        Settings m_settings;
        
        GLBackground m_glBackground;
        GLPolygons m_glPolygons;
        GLScenery m_glScenery;
        GLSpawnPoints m_glSpawnPoints;

        GLOutlinePolygons m_glOutlinePolygons;
        GLOutlineScenery m_glOutlineScenerySelection;
        GLOutlineScenery m_glOutlineSceneryWireframe;

        GLSelectionPolygons m_glSelectionPolygons;

        bool m_textureTransformationMode;
};

#endif
