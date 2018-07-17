#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "glfunctions.hpp"
#include <wx/glcanvas.h>
#include "camera.hpp"
#include "displaysettings.hpp"
#include "glmanager.hpp"
#include "../map/map.hpp"
#include "../selection.hpp"
#include "../settings.hpp"
#include "../canvas.hpp"

class MainFrame;

/**
 * \brief Custom implementation of OpenGL canvas.
 */
class GLCanvas: public wxGLCanvas, public Canvas
{
    public:
        GLCanvas(wxWindow *parent,
            MainFrame& mainFrame,
            Settings settings,
            const DisplaySettings& displaySettings,
            const wxGLAttributes &glCanvasAttributes,
            const PolygonSelection& polygonSelection,
            const Selection& Sceneryelection,
            Map &map
        );
        virtual ~GLCanvas() = default;

        PMSVertex CreateVertex(wxColor color, wxPoint point);

        int AddPolygon(PMSPolygon polygon, PMSVertex firstVertex) override;
        void RemovePolygons(const wxVector<unsigned int> &polygons) override;
        int AddSpawnPoint(PMSSpawnPoint spawnPoint) override;
        void EditPolygonVertex(unsigned polygonIndex, PMSPolygonType polygonType, unsigned vertexIndex, PMSVertex vertex) override;
        void EditScenery(unsigned sceneryIndex, PMSScenery scenery) override;
        void EditSpawnPoint(unsigned int spawnPointIdx, PMSSpawnPoint spawnPoint) override;

        void RemoveSpawnPoints(wxVector<unsigned int> spawnPointIndexes) override;

        const PMSPolygon& GetPolygon(unsigned polygonIndex) const override;
        void UpdatePolygonSelectionForRedraw() override;

        void RemoveSceneries(const wxVector<unsigned int> &sceneries) override;
        void RemoveScenery(unsigned int sceneryId) override;
        int AddScenery(PMSScenery newScenery) override;
        int AddSceneryType(wxString sceneryName) override;

        void PopupMenu(wxMenu* menu);

        void HandleLeftMouseButtonClick(const wxMouseEvent& event) override;
        void HandleMouseMotion(const wxMouseEvent &event) override;
        void HandleRightMouseButtonRelease(const wxMouseEvent& event) override;
        void HandleKeyPress(const wxKeyEvent &event) override;

        void SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor) override;
        void SetPolygonsTexture(wxString textureFilename) override;
        void SetTextureTransformationMode(bool mode = true) override;

        unsigned GetPolygonCount() const override { return m_map.GetPolygonsCount(); }

        void Draw() override;

        wxRealPoint GetMousePositionOnMap(wxPoint mousePositionOnCanvas) const override;
    private:
        Camera m_camera;
        GLManager m_glManager;
        Map &m_map;
        MainFrame& m_mainFrame;
        wxRealPoint m_mousePositionOnMap;
        const DisplaySettings& m_displaySettings;
        const PolygonSelection& m_polygonSelection;
        const Selection& m_scenerySelection;

        void OnMouseWheel(wxMouseEvent &event);
        void OnNewPolygonTypeSelected(wxCommandEvent &event);
        void OnPaint(wxPaintEvent &event);
        void OnResize(wxSizeEvent &event);
        void InitGL();
};
