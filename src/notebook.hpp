#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/notebook.h>
#include "gl/glcanvas.hpp"
#include "gl/displaysettings.hpp"
#include "workspace.hpp"
#include "settings.hpp"

/**
 * \brief Represents a notebook with multiple pages/tabs. In our notebook, pages
 *  are instances of Workspace class.
 */
class Notebook: public wxNotebook
{
    public:
        Notebook(wxWindow *parent);
        void AddWorkspace(Settings settings, wxString mapPath);

        DisplaySettings GetCurrentDisplaySettings();
        void SetCurrentDisplaySetting(int setting, bool display);

        GLCanvas *GetCurrentGLCanvas();
        wxPoint GetCurrentMousePositionOnMap();

        void HandleCurrentGLCanvasLeftMouseButtonClick(int selectedToolId);
        void HandleCurrentGLCanvasRightMouseButtonRelease(int selectedToolId);

        void SaveCurrentMapAsPMS(wxString destinationPath);

        void SelectAll();

    private:
        Workspace *GetCurrentWorkspace();
        void OnPageChanged(wxBookCtrlEvent &event);
};

#endif
