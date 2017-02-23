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

void GLCanvas::EditPolygonVertex(unsigned polygonIndex, PMSPolygonType polygonType,
    unsigned vertexIndex, PMSVertex vertex)
{
    m_map.EditPolygonVertex(polygonIndex, vertexIndex, vertex);
    m_glManager.EditPolygonVertex(polygonIndex, polygonType, vertexIndex, vertex);
}

const PMSPolygon& GLCanvas::GetPolygon(unsigned polygonIndex) const
{
    return m_map.GetPolygons()[polygonIndex];
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
        m_camera.ScrollX(m_mousePositionOnMap.x - newMousePositionOnMap.x);
        m_camera.ScrollY(m_mousePositionOnMap.y - newMousePositionOnMap.y);
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

    if (wheelRotation > 0 && m_camera.CanZoomIn())
    {
        m_camera.ZoomIn();
        Refresh();
    }
    else if (wheelRotation < 0 && m_camera.CanZoomOut())
    {
        m_camera.ZoomOut();
        Refresh();
    }
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
