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

/**
 * \brief Custom implementation of OpenGL canvas.
 */
class GLCanvas: public wxGLCanvas
{
    public:
        GLCanvas(wxWindow *parent, Settings settings, const wxGLAttributes &glCanvasAttributes, Map *map);
        virtual ~GLCanvas();

        DisplaySettings GetDisplaySettings() { return m_displaySettings; }
        void SetDisplaySetting(int setting, bool display);

        wxPoint GetMousePositionOnMap() { return m_mousePositionOnMap; }

        void HandleLeftMouseButtonClick(wxPoint mousePositionOnCanvas, int selectedToolId);
        void HandleRightMouseButtonRelease(int selectedToolId);

        void SelectAll();

    private:
        Camera m_camera;
        DisplaySettings m_displaySettings;
        GLManager *m_glManager;
        Map *m_map;
        wxPoint m_mousePositionOnCanvas,
                m_mousePositionOnMap;
        Selection m_selectedPolygons, m_selectedScenery;

        // Indicates how many vertices have been set while adding a new polygon.
        unsigned int m_addedPolygonVerticesCount;
        PMSPolygonType m_newPolygonType;
        // \brief Is user currently creating a new polygon?
        bool AddingPolygon() { return m_addedPolygonVerticesCount > 0; }
        // \brief Returns PMSVertex with coordinates of cursor on map.
        PMSVertex CreateVertexOnMouse();

        void OnMouseMotion(wxMouseEvent &event);
        void OnMouseWheel(wxMouseEvent &event);
        void OnNewPolygonTypeSelected(wxCommandEvent &event);
        void OnPaint(wxPaintEvent &event);
        void OnResize(wxSizeEvent &event);

        wxPoint GetMousePositionOnMap(wxPoint mousePositionOnCanvas);
        void InitGL();
};

#endif
