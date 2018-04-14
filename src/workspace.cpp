#include "workspace.hpp"
#include "gl/glcanvas.hpp"
#include "gl/displaysettings.hpp"
#include "eventdispatcherfactory.hpp"
#include <wx/glcanvas.h>
#include <utility>
#include "mainframe.hpp"

Workspace::Workspace(wxWindow *notebook, MainFrame& mainFrame, Settings settings, wxString mapPath)
    : wxWindow(notebook, wxID_ANY)
    , m_map{mapPath}
    , m_sceneryFrame{mainFrame.GetSceneryFrame()}
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
    
    m_selectionManager = std::make_unique<SelectionManager>(*m_glCanvas, m_displaySettings, mainFrame.GetSceneryFrame(), std::move(polygonSelection), std::move(scenerySelection));
    m_eventDispatcher = CreateEventDispatcher(*m_glCanvas, mainFrame, *m_selectionManager);
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

void Workspace::GiveFocusToGLCanvas()
{
    m_glCanvas->SetFocus();

    m_sceneryFrame.ClearSceneryList();
    const auto& sceneryInstances = m_map.GetSceneryInstances();
    for (int i = 0; i < sceneryInstances.size(); i++)
    {
        m_sceneryFrame.AddScenery(wxString(m_map.GetSceneryTypes()[sceneryInstances[i].style - 1].name), i, sceneryInstances[i].style);
    }
}

void Workspace::SaveMapAsPMS(const wxString& destinationPath)
{
    // We unselect current tool so that temporary map objects don't get saved to output file.
    int selectedToolId = m_eventDispatcher->Unselect();
    m_map.SaveMapAsPMS(destinationPath);
    m_eventDispatcher->Select(selectedToolId);
}

void Workspace::SelectAll()
{
    m_selectionManager->SelectAll();
}

void Workspace::SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor)
{
    m_glCanvas->SetBackgroundColors(backgroundBottomColor, backgroundTopColor);
}

void Workspace::SetPolygonsTexture(const wxString& textureFilename)
{
    m_glCanvas->SetPolygonsTexture(textureFilename);
}

void Workspace::SelectTool(int toolId)
{
    m_eventDispatcher->Select(toolId);
}
