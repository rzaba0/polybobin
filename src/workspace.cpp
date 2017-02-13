#include "workspace.hpp"
#include "gl/glcanvas.hpp"
#include "eventdispatcherfactory.hpp"
#include <wx/glcanvas.h>

Workspace::Workspace(wxWindow *notebook, MainFrame& mainFrame, Settings settings, wxString mapPath)
    : wxWindow(notebook, wxID_ANY)
    , m_map{mapPath}
{
    wxGLAttributes glCanvasAttributes;
    glCanvasAttributes.PlatformDefaults().Defaults().EndList();

    bool accepted = wxGLCanvas::IsDisplaySupported(glCanvasAttributes);
    if (!accepted)
    {
        throw std::runtime_error("OpenGL display attributes are not supported. The program will quit now.");
    }

    m_glCanvas = new GLCanvas(this, mainFrame, settings, glCanvasAttributes, m_map);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    sizer->Add(m_glCanvas, 1, wxEXPAND);
    m_eventDispatcher = MakeEventDispatcher(*m_glCanvas, mainFrame);
    m_eventDispatcher->Select(1);
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

void Workspace::SaveMapAsPMS(const wxString& destinationPath)
{
    m_map.SaveMapAsPMS(destinationPath);
}

void Workspace::SelectAll()
{

}

void Workspace::SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor)
{
    m_glCanvas->SetBackgroundColors(backgroundBottomColor, backgroundTopColor);
}

void Workspace::SetPolygonsTexture(const wxString& textureFilename)
{
    m_glCanvas->SetPolygonsTexture(textureFilename);
}