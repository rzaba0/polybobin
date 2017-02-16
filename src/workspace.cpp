#include "workspace.hpp"
#include "gl/glcanvas.hpp"
#include "gl/displaysettings.hpp"
#include "eventdispatcherfactory.hpp"
#include <wx/glcanvas.h>
#include <utility>

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
    auto polygonSelection = std::make_unique<PolygonSelection>();
    auto scenerySelection = std::make_unique<Selection>();
    m_glCanvas = new GLCanvas(this, mainFrame, settings, m_displaySettings, glCanvasAttributes, *polygonSelection, *scenerySelection, m_map);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    m_eventDispatcher = MakeEventDispatcher(*m_glCanvas, mainFrame);
    m_eventDispatcher->Select(3);
    m_selectionManager = std::make_unique<SelectionManager>(*m_glCanvas, m_displaySettings, std::move(polygonSelection), std::move(scenerySelection));
    sizer->Add(m_glCanvas, 1, wxEXPAND);
}

const DisplaySettings& Workspace::GetDisplaySettings()
{
    return m_displaySettings;
}

void Workspace::SetDisplaySetting(int setting, bool display)
{
    m_displaySettings.SetDisplaySetting(setting, display);
    m_glCanvas->Refresh();
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