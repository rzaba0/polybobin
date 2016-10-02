#include "notebook.hpp"

Notebook::Notebook(wxWindow *parent)
    : wxNotebook(parent, wxID_ANY)
{
}

void Notebook::AddWorkspace(wxString mapPath)
{
    Workspace *workspace;
    workspace = new Workspace(this, mapPath);

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

void Notebook::SelectAll()
{
    GetCurrentWorkspace()->SelectAll();
}

Workspace *Notebook::GetCurrentWorkspace()
{
    return (Workspace*) GetCurrentPage();
}