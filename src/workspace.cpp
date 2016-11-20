#include "workspace.hpp"
#include "gl/glcanvas.hpp"
#include <wx/glcanvas.h>

Workspace::Workspace(wxWindow *notebook, Settings settings, wxString mapPath)
    : wxWindow(notebook, wxID_ANY)
{
    wxGLAttributes glCanvasAttributes;
    glCanvasAttributes.PlatformDefaults().Defaults().EndList();

    bool accepted = wxGLCanvas::IsDisplaySupported(glCanvasAttributes);
    if (!accepted)
    {
        throw (wxString) wxT("OpenGL display attributes are not supported. The program will quit now.");
    }

    m_map = new Map(mapPath);

    m_glCanvas = new GLCanvas(this, settings, glCanvasAttributes, m_map);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    sizer->Add(m_glCanvas, 1, wxEXPAND);
}

Workspace::~Workspace()
{
    delete m_map;
}

DisplaySettings Workspace::GetDisplaySettings()
{
    return m_glCanvas->GetDisplaySettings();
}

void Workspace::SetDisplaySetting(int setting, bool display)
{
    m_glCanvas->SetDisplaySetting(setting, display);
}

wxPoint Workspace::GetMousePositionOnMap()
{
    return m_glCanvas->GetMousePositionOnMap();
}

void Workspace::GiveFocusToGLCanvas()
{
    m_glCanvas->SetFocus();
}

void Workspace::HandleGLCanvasLeftMouseButtonClick(int selectedToolId)
{
    m_glCanvas->HandleLeftMouseButtonClick(selectedToolId);
}

void Workspace::HandleGLCanvasRightMouseButtonRelease(int selectedToolId)
{
    m_glCanvas->HandleRightMouseButtonRelease(selectedToolId);
}

void Workspace::SaveMapAsPMS(wxString destinationPath)
{
    m_map->SaveMapAsPMS(destinationPath);
}

void Workspace::SelectAll()
{
    m_glCanvas->SelectAll();
}