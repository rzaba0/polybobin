#include "glcanvas.hpp"
#include "../app.hpp"
#include "../constants.hpp"
#include "../mainframe.hpp"

GLCanvas::GLCanvas(wxWindow *parent, MainFrame& mainFrame, Settings settings, const wxGLAttributes &glCanvasAttributes, Map &map)
    : wxGLCanvas(parent, glCanvasAttributes, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW | wxEXPAND)
    , m_map(map)
    , m_glManager(settings, map)
    , m_mainFrame(mainFrame)
{
    wxGLContextAttrs glContextAttributes;
    glContextAttributes.PlatformDefaults().CoreProfile().EndList();
    m_movingSelected = false;

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

void GLCanvas::PopupMenu(wxMenu* menu)
{
    wxWindow::PopupMenu(menu);
}

void GLCanvas::SetDisplaySetting(int setting, bool display)
{
    m_displaySettings.SetDisplaySetting(setting, display);
    Refresh();
}

void GLCanvas::HandleLeftMouseButtonClick(const wxMouseEvent &event)
{
    m_mousePositionOnCanvas = event.GetPosition();
    m_mousePositionOnMap = GetMousePositionOnMap(m_mousePositionOnCanvas);
}

void GLCanvas::HandleMouseMotion(const wxMouseEvent &event)
{
    wxPoint newMousePositionOnCanvas = event.GetPosition();

    wxPoint oldMousePositionOnMap = GetMousePositionOnMap(m_mousePositionOnCanvas);
    wxPoint newMousePositionOnMap = GetMousePositionOnMap(newMousePositionOnCanvas);

    if (event.MiddleIsDown() && event.Dragging())
    {
        m_camera.ScrollX((float) (oldMousePositionOnMap.x - newMousePositionOnMap.x));
        m_camera.ScrollY((float) (oldMousePositionOnMap.y - newMousePositionOnMap.y));

        Refresh();
    }

    if (m_movingSelected)
    {
        bool moveSelectedKeyPressed = wxGetKeyState(MOVE_SELECTED_KEY);

        if (!event.LeftIsDown() || !moveSelectedKeyPressed)
        {
            m_movingSelected = false;
        }

        if (event.LeftIsDown() && event.Dragging())
        {
            float mouseDiffX = newMousePositionOnMap.x - oldMousePositionOnMap.x;
            float mouseDiffY = newMousePositionOnMap.y - oldMousePositionOnMap.y;
            const auto &polygons = m_map.GetPolygons();
            const auto &sceneries = m_map.GetSceneryInstances();
            const auto &selectedPolygons = m_selectedPolygons.GetSelectedIds();
            const auto &selectedSceneries = m_selectedScenery.GetSelectedIds();

            for (size_t i = 0; i < selectedPolygons.size(); ++i)
            {
                unsigned int polygonId = selectedPolygons[i];
                PMSPolygon polygon = polygons.at(polygonId);
                for (unsigned j = 0; j < 3; ++j)
                {
                    PMSVertex vertex = polygon.vertices[j];
                    vertex.x += mouseDiffX;
                    vertex.y += mouseDiffY;
                    m_map.EditPolygonVertex(polygonId, j, vertex);
                    m_glManager.EditPolygonVertex(polygonId, polygon.polygonType, j, vertex);
                }
            }

            for (size_t i = 0; i < selectedSceneries.size(); ++i)
            {
                unsigned int sceneryId = selectedSceneries[i];
                PMSScenery scenery = sceneries.at(sceneryId);
                scenery.x += mouseDiffX;
                scenery.y += mouseDiffY;
                m_map.EditScenery(sceneryId, scenery);
                m_glManager.EditScenery(sceneryId, scenery);
            }
            Refresh();
        }
    }

    m_mousePositionOnCanvas = newMousePositionOnCanvas;
    m_mousePositionOnMap = GetMousePositionOnMap(m_mousePositionOnCanvas);
}

void GLCanvas::HandleRightMouseButtonRelease(const wxMouseEvent &event)
{
}

void GLCanvas::SelectAll()
{
    m_selectedPolygons.SelectAll(m_map.GetPolygons().size());
    m_selectedScenery.SelectAll(m_map.GetSceneryInstances().size());
    Refresh();
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

    m_glManager.Render(m_camera, this->GetSize(), m_displaySettings,
        m_selectedPolygons, m_selectedScenery, //AddingPolygon());
        false);
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


wxPoint GLCanvas::GetMousePositionOnMap(wxPoint mousePositionOnCanvas)
{
    wxPoint result;
    wxSize canvasSize = GetSize();

    /**
     * Proportion from which we take the formula:
     * mousePositionOnCanvas.x : canvasSize.GetWidth() = result.x : m_camera.GetWidth()
     */

    result.x = (int) (m_camera.GetX() +
                     (float) mousePositionOnCanvas.x * m_camera.GetWidth(canvasSize) / (float) canvasSize.GetWidth());

    result.y = (int) (m_camera.GetY() +
                     (float) mousePositionOnCanvas.y * m_camera.GetHeight(canvasSize) / (float)canvasSize.GetHeight());

    return result;
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
