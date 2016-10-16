#include "notebook.hpp"

Notebook::Notebook(wxWindow *parent)
    : wxNotebook(parent, wxID_ANY)
{
    /**
     * Looks like on Windows instances of wxGLCanvas don't get focus by default,
     * which means it's impossible to zoom in/out. To fix it, we give focus to
     * current wxGLCanvas every time the user changes page in notebook.
     */
    Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &Notebook::OnPageChanged, this);
}

void Notebook::AddWorkspace(Settings settings, wxString mapPath)
{
    Workspace *workspace;
    workspace = new Workspace(this, settings, mapPath);

    wxString title = mapPath.IsEmpty() ? "Untitled" : mapPath.AfterLast('/');

    AddPage(workspace, title, true);
}

DisplaySettings Notebook::GetCurrentDisplaySettings()
{
    return GetCurrentWorkspace()->GetDisplaySettings();
}

void Notebook::SetCurrentDisplaySetting(int setting, bool display)
{
    GetCurrentWorkspace()->SetDisplaySetting(setting, display);
}

GLCanvas *Notebook::GetCurrentGLCanvas()
{
    return GetCurrentWorkspace()->GetGLCanvas();
}

wxPoint Notebook::GetCurrentMousePositionOnMap()
{
    return GetCurrentWorkspace()->GetMousePositionOnMap();
}

void Notebook::HandleCurrentGLCanvasClick(int selectedToolId)
{
    GetCurrentWorkspace()->HandleGLCanvasClick(selectedToolId);
}

void Notebook::SaveCurrentMapAsPMS(wxString destinationPath)
{
    GetCurrentWorkspace()->SaveMapAsPMS(destinationPath);
}

void Notebook::SelectAll()
{
    GetCurrentWorkspace()->SelectAll();
}

Workspace *Notebook::GetCurrentWorkspace()
{
    return (Workspace*) GetCurrentPage();
}

void Notebook::OnPageChanged(wxBookCtrlEvent &event)
{
    GetCurrentWorkspace()->GiveFocusToGLCanvas();
}