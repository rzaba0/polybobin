#include <cmath>
#include <string>
#include "glcanvas.hpp"
#include "../app.hpp"
#include "../constants.hpp"
#include "../mainframe.hpp"

GLCanvas::GLCanvas(wxWindow *parent,
        MainFrame& mainFrame,
        Settings settings,
        const DisplaySettings& displaySettings,
        const wxGLAttributes &glCanvasAttributes,
        const PolygonSelection& polygonSelection,
        const Selection& scenerySelection,
        Map &map
    )
    : wxGLCanvas(parent, glCanvasAttributes, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW | wxEXPAND)
    , m_glManager(settings, map)
    , m_map(map)
    , m_mainFrame(mainFrame)
    , m_displaySettings{displaySettings}
    , m_polygonSelection{polygonSelection}
    , m_scenerySelection{scenerySelection}
{
    wxGLContextAttrs glContextAttributes;
    glContextAttributes.PlatformDefaults().CoreProfile().EndList();

    Bind(wxEVT_MOUSEWHEEL, &GLCanvas::OnMouseWheel, this);
    Bind(wxEVT_PAINT, &GLCanvas::OnPaint, this);
    Bind(wxEVT_SIZE, &GLCanvas::OnResize, this);
}

int GLCanvas::AddPolygon(PMSPolygon polygon, PMSVertex firstVertex)
{
    auto id = m_map.AddPolygon(polygon);
    m_glManager.AddPolygon(polygon.polygonType, firstVertex);
    return id;
}

void GLCanvas::RemovePolygons(const wxVector<unsigned int> &polygons)
{
    m_map.RemovePolygons(polygons);
    m_glManager.ResetPolygons(m_map.GetPolygons());
}

int GLCanvas::AddSpawnPoint(PMSSpawnPoint spawnPoint)
{
    auto id = m_map.AddSpawnPoint(spawnPoint);
    m_glManager.AddSpawnPoint(spawnPoint);
    return id;
}

void GLCanvas::EditPolygonVertex(unsigned polygonIndex, PMSPolygonType polygonType,
    unsigned vertexIndex, PMSVertex vertex)
{
    m_map.EditPolygonVertex(polygonIndex, vertexIndex, vertex);
    m_glManager.EditPolygonVertex(polygonIndex, polygonType, vertexIndex, vertex);
}

void GLCanvas::EditScenery(unsigned sceneryIndex, PMSScenery scenery)
{
    m_map.EditScenery(sceneryIndex, scenery);
    m_glManager.EditScenery(sceneryIndex, scenery);
}

void GLCanvas::EditSpawnPoint(unsigned int spawnPointIdx, PMSSpawnPoint spawnPoint)
{
    m_map.EditSpawnPoint(spawnPointIdx, spawnPoint);
    m_glManager.EditSpawnPoint(spawnPointIdx, spawnPoint);
}

void GLCanvas::RemoveSpawnPoints(wxVector<unsigned int> spawnPointIndexes)
{
    m_map.RemoveSpawnPoints(spawnPointIndexes);
    m_glManager.ResetSpawnPoints(m_map.GetSpawnPoints());
}

const PMSPolygon& GLCanvas::GetPolygon(unsigned polygonIndex) const
{
    return m_map.GetPolygons()[polygonIndex];
}

const PMSScenery& GLCanvas::GetScenery(unsigned sceneryIndex) const
{
    return m_map.GetSceneryInstances()[sceneryIndex];
}

void GLCanvas::RemoveSceneries(const wxVector<unsigned int> &sceneries)
{
    m_map.RemoveSceneries(sceneries);
    m_glManager.ResetSceneries(m_map.GetSceneryInstances());
    m_map.RemoveUnusedSceneryTypes();
}

void GLCanvas::RemoveScenery(unsigned int sceneryId)
{
    wxVector<unsigned int> sceneries;
    sceneries.push_back(sceneryId);
    RemoveSceneries(sceneries);
}

int GLCanvas::AddScenery(PMSScenery newScenery)
{
    int newSceneryId = m_map.AddScenery(newScenery);
    m_glManager.ResetSceneries(m_map.GetSceneryInstances());
    m_map.RemoveUnusedSceneryTypes();

    return newSceneryId;
}

int GLCanvas::AddSceneryType(wxString sceneryName)
{
    int sceneryTypesNum = m_map.GetSceneryTypes().size();

    int newSceneryTypeId = m_map.AddSceneryType(sceneryName);

    if (sceneryTypesNum != m_map.GetSceneryTypes().size()) // New SceneryType
    {
        m_glManager.AddSceneryTexture(sceneryName);
    }

    return newSceneryTypeId;
}

void GLCanvas::PopupMenu(wxMenu* menu)
{
    wxWindow::PopupMenu(menu);
}

void GLCanvas::HandleLeftMouseButtonClick(const wxMouseEvent &event)
{
    m_mousePositionOnMap = GetMousePositionOnMap(event.GetPosition());
}

void GLCanvas::HandleMouseMotion(const wxMouseEvent &event)
{
    wxRealPoint newMousePositionOnMap = GetMousePositionOnMap(event.GetPosition());
    if (event.MiddleIsDown() && event.Dragging())
    {
        m_camera.Scroll(m_mousePositionOnMap - newMousePositionOnMap);
        Refresh();
    }
    m_mousePositionOnMap = GetMousePositionOnMap(event.GetPosition());
    std::string mousePosition = std::to_string(int(round(m_mousePositionOnMap.x)))
        + " " + std::to_string(int(round(m_mousePositionOnMap.y)));
    m_mainFrame.SetStatusText(mousePosition);
}

void GLCanvas::HandleRightMouseButtonRelease(const wxMouseEvent &event)
{
}

void GLCanvas::HandleKeyPress(const wxKeyEvent &event)
{
}

void GLCanvas::SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor)
{
    m_glManager.SetBackgroundColors(backgroundBottomColor, backgroundTopColor);
    Refresh();
}

void GLCanvas::SetPolygonsTexture(wxString textureFilename)
{
    m_glManager.SetPolygonsTexture(textureFilename);
    Refresh();
}

void GLCanvas::SetFillPolygonSelection(bool mode)
{
    m_glManager.SetFillPolygonSelection(m_polygonSelection, mode);
    Refresh();
}

void GLCanvas::SetTransformFrameVisible(bool mode)
{
    m_glManager.SetTransformFrameVisible(mode);
    Refresh();
}

void GLCanvas::SetTransformFramePosition(wxPoint bottomLeft,
                               wxPoint bottomRight,
                               wxPoint topLeft,
                               wxPoint topRight)
{
    m_glManager.SetTransformFramePosition(bottomLeft, bottomRight, topLeft, topRight);
    Refresh();
}

void GLCanvas::Draw()
{
    Refresh();
}

void GLCanvas::UpdatePolygonSelectionForRedraw()
{
    m_glManager.ApplyPolygonSelection(m_polygonSelection);
}

// TODO: move this functionality to suitable place
// For now it's kind of bound here by m_glManager
PMSVertex GLCanvas::CreateVertex(wxColor color, wxPoint canvasPoint)
{
    auto point = GetMousePositionOnMap(canvasPoint);
    float textureWidth = (float) m_glManager.GetTextureWidth(),
        textureHeight = (float) m_glManager.GetTextureHeight();
    PMSVertex vertex;
    vertex.x = point.x;
    vertex.y = point.y;
    vertex.z = 1.0f;
    vertex.rhw = 1.0f;
    vertex.color = color;
    vertex.textureS = (float) point.x / textureWidth;
    vertex.textureT = (float) point.y / textureHeight;
    return vertex;
}

void GLCanvas::OnMouseWheel(wxMouseEvent &event)
{
    int wheelRotation = event.GetWheelRotation();

    auto p0 = GetMousePositionOnMap(event.GetPosition());
    if (wheelRotation > 0 && m_camera.CanZoomIn())
    {
        m_camera.ZoomIn();
    }
    else if (wheelRotation < 0 && m_camera.CanZoomOut())
    {
        m_camera.ZoomOut();
    }
    auto p1 = GetMousePositionOnMap(event.GetPosition());
    m_camera.Scroll(p0 - p1);
    Refresh();
}

void GLCanvas::OnPaint(wxPaintEvent &event)
{
    wxPaintDC(this);

    if (!m_glManager.IsGLReady())
    {
        InitGL();
    }

    wxGetApp().GetGLContext(this);

    m_glManager.Render(m_camera, this->GetSize(), m_displaySettings, m_polygonSelection, m_scenerySelection, true);
    SwapBuffers();
}

void GLCanvas::OnResize(wxSizeEvent &event)
{
    event.Skip();

    if (!IsShownOnScreen())
    {
        return;
    }

    if (!m_glManager.IsGLReady())
    {
        InitGL();
    }

    wxSize canvasSize = this->GetSize();
    glViewport(0, 0, canvasSize.GetWidth(), canvasSize.GetHeight());

    Refresh(false);
}


wxRealPoint GLCanvas::GetMousePositionOnMap(wxPoint mousePositionOnCanvas) const
{
    wxSize canvasSize = GetSize();

    /**
     * Proportion from which we take the formula:
     * mousePositionOnCanvas.x : canvasSize.GetWidth() = result.x : m_camera.GetWidth()
     */
    double x = m_camera.GetX() + double(mousePositionOnCanvas.x) * m_camera.GetWidth(canvasSize) / double(canvasSize.GetWidth());
    double y = m_camera.GetY() + double(mousePositionOnCanvas.y) * m_camera.GetHeight(canvasSize) / double(canvasSize.GetHeight());
    return {x, y};
}

wxRealPoint GLCanvas::GetMapPositionOnScreen(wxRealPoint mapPosition) const
{
    wxSize canvasSize = GetSize();

    /**
     * Proportion from which we take the formula:
     * mousePositionOnCanvas.x : canvasSize.GetWidth() = result.x : m_camera.GetWidth()
     */
    double x = (mapPosition.x - m_camera.GetX()) * double(canvasSize.GetWidth()) / m_camera.GetWidth(canvasSize);
    double y = (mapPosition.y - m_camera.GetY()) * double(canvasSize.GetHeight()) / m_camera.GetHeight(canvasSize);

    return {x, y};
}

void GLCanvas::InitGL()
{
    wxGetApp().GetGLContext(this);

    try
    {
        m_glManager.Init();
        m_glManager.SetupShaders();
        m_glManager.SetupTextures();
    }
    catch (const std::exception &error)
    {
        // TODO: handle properly.
    }

    m_glManager.SetupVertices();
}
