#ifndef WORKSPACE_HPP
#define WORKSPACE_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "gl/displaysettings.hpp"
#include "gl/glcanvas.hpp"
#include "map/map.hpp"
#include "settings.hpp"
#include <memory>

/**
 * \brief Represents a page inside our Notebook class.
 *  It contains OpenGL canvas in which we draw the map.
 */
class Workspace: public wxWindow
{
    public:
        Workspace(wxWindow *notebook, Settings settings, wxString mapPath);
        ~Workspace() = default;

        DisplaySettings GetDisplaySettings();
        void SetDisplaySetting(int setting, bool display);

        GLCanvas *GetGLCanvas() { return m_glCanvas; }
        wxPoint GetMousePositionOnMap();

        void GiveFocusToGLCanvas();
        void HandleGLCanvasLeftMouseButtonClick(wxPoint mousePositionOnCanvas, int selectedToolId,
                                                wxColor selectedColor);
        void HandleGLCanvasMouseMotion(wxMouseEvent &event, wxColor selectedColor);
        void HandleGLCanvasRightMouseButtonRelease(int selectedToolId);

        void SaveMapAsPMS(wxString destinationPath);

        void SelectAll();

    private:
        GLCanvas *m_glCanvas;
        std::unique_ptr<Map> m_map;
};

#endif
