#include "notebook.hpp"

Notebook::Notebook(wxWindow *parent, MainFrame& mainFrame)
    : wxNotebook(parent, wxID_ANY)
    , m_mainFrame(mainFrame)
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
    Workspace *workspace = new Workspace(this, m_mainFrame, settings, mapPath);
    wxString title = mapPath.IsEmpty() ? "Untitled" : mapPath.AfterLast('/');
    AddPage(workspace, title, true);
}

void Notebook::SetCurrentDisplaySetting(int setting, bool display)
{
    GetCurrentWorkspace().SetDisplaySetting(setting, display);
}

GLCanvas &Notebook::GetCurrentGLCanvas()
{
    return GetCurrentWorkspace().GetGLCanvas();
}

const DisplaySettings& Notebook::GetCurrentDisplaySettings()
{
    return GetCurrentWorkspace().GetDisplaySettings();
}

Map &Notebook::GetCurrentMap()
{
    return GetCurrentWorkspace().GetMap();
}

wxPoint Notebook::GetCurrentMousePositionOnMap()
{
    return GetCurrentWorkspace().GetMousePositionOnMap();
}
void Notebook::SaveCurrentMapAsPMS(const wxString& destinationPath)
{
    GetCurrentWorkspace().SaveMapAsPMS(destinationPath);
}

void Notebook::SelectAll()
{
    GetCurrentWorkspace().SelectAll();
}

void Notebook::SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor)
{
    GetCurrentWorkspace().SetBackgroundColors(backgroundBottomColor, backgroundTopColor);
}

void Notebook::SetPolygonsTexture(const wxString& textureFilename)
{
    GetCurrentWorkspace().SetPolygonsTexture(textureFilename);
}

void Notebook::OnPageChanged(wxBookCtrlEvent &event)
{
    GetCurrentWorkspace().GiveFocusToGLCanvas();
}

Workspace& Notebook::GetCurrentWorkspace()
{
    return *static_cast<Workspace*>(GetCurrentPage());
}