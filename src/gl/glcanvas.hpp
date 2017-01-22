#ifndef GLCANVAS_HPP
#define GLCANVAS_HPP

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

class MainFrame;

/**
 * \brief Custom implementation of OpenGL canvas.
 */
class GLCanvas: public wxGLCanvas
{
    public:
        GLCanvas(wxWindow *parent, MainFrame& mainFrame, Settings settings, const wxGLAttributes &glCanvasAttributes, Map &map);
        virtual ~GLCanvas() = default;

        DisplaySettings GetDisplaySettings() { return m_displaySettings; }
        void SetDisplaySetting(int setting, bool display);
        wxPoint GetMousePositionOnMap() { return m_mousePositionOnMap; }

        PMSVertex CreateVertex(wxColor color, wxPoint point);

        int AddPolygon(PMSPolygon polygon, PMSVertex firstVertex);
        void EditPolygonVertex(unsigned polygonIndex, PMSPolygonType polygonType,
            unsigned vertexIndex, PMSVertex vertex);
        void GLCanvas::PopupMenu(wxMenu* menu);

        void HandleLeftMouseButtonClick(const wxMouseEvent& event);
        void HandleMouseMotion(const wxMouseEvent &event);
        void HandleRightMouseButtonRelease(const wxMouseEvent& event);

        void SelectAll();
        void SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor);
        void SetPolygonsTexture(wxString textureFilename);

    private:
        Camera m_camera;
        DisplaySettings m_displaySettings;
        GLManager m_glManager;
        Map &m_map;
        MainFrame& m_mainFrame;
        wxPoint m_mousePositionOnCanvas,
                m_mousePositionOnMap;
        Selection m_selectedPolygons, m_selectedScenery;
        bool m_movingSelected;

        void OnMouseWheel(wxMouseEvent &event);
        void OnNewPolygonTypeSelected(wxCommandEvent &event);
        void OnPaint(wxPaintEvent &event);
        void OnResize(wxSizeEvent &event);

        wxPoint GetMousePositionOnMap(wxPoint mousePositionOnCanvas);
        void InitGL();
};

#endif
