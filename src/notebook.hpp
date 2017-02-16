#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/notebook.h>
#include "gl/glcanvas.hpp"
#include "gl/displaysettings.hpp"
#include "map/map.hpp"
#include "settings.hpp"
#include "workspace.hpp"

class MainFrame;

/**
 * \brief Represents a notebook with multiple pages/tabs. In our notebook, pages
 *  are instances of Workspace class.
 */
class Notebook: public wxNotebook
{
    public:
        Notebook(wxWindow *parent, MainFrame& mainFrame);
        void AddWorkspace(Settings settings, wxString mapPath);
       
        const DisplaySettings& GetCurrentDisplaySettings();
        GLCanvas& GetCurrentGLCanvas();
        Map& GetCurrentMap();
        wxPoint GetCurrentMousePositionOnMap();

        void SetCurrentDisplaySetting(int setting, bool display);
        void SaveCurrentMapAsPMS(const wxString& destinationPath);
        void SelectAll();
        void SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor);
        void SetPolygonsTexture(const wxString& textureFilename);

    private:
        MainFrame& m_mainFrame;
        Workspace& GetCurrentWorkspace();
        void OnPageChanged(wxBookCtrlEvent &event);
};

#endif
