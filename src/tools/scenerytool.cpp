#include "scenerytool.hpp"
#include "../gl/image.hpp"

SceneryTool::SceneryTool(GLCanvas& canvas, const Settings& settings,
            SceneryFrame& sceneryFrame, const PaletteFrame& palette)
    : m_canvas(canvas)
    , m_settings(settings)
    , m_sceneryFrame(sceneryFrame)
    , m_palette(palette)
    , m_toolSelected(false)
    , m_currentSceneryTypeName("None")
    , m_editingScenery(false)
    , m_movingScenery(false)
    , m_rotatingScenery(false)
    , m_scalingScenery(false)
    , m_sceneryId(0)
{
    sceneryFrame.GetSceneryListCtrl()->Bind(wxEVT_LIST_ITEM_SELECTED, &SceneryTool::OnSceneryFrameSelect, this);
}

void SceneryTool::OnSelect()
{
    m_toolSelected = true;

    m_editingScenery = false;
    m_movingScenery = false;
    m_rotatingScenery = false;
    m_scalingScenery = false;
}

void SceneryTool::OnUnselect()
{
    m_toolSelected = false;

    if (m_movingScenery)
    {
        m_canvas.RemoveScenery(m_sceneryId);
        m_canvas.Refresh();
    }

    m_editingScenery = false;
    m_movingScenery = false;
    m_rotatingScenery = false;
    m_scalingScenery = false;
}

void SceneryTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    if (m_editingScenery)
    {
        if (m_scalingScenery)
        {
            m_scalingScenery = false;
        }

        if (m_rotatingScenery)
        {
            m_rotatingScenery = false;

            if (m_sceneryFrame.GetScale())
                m_scalingScenery = true;
        }

        if (m_movingScenery)
        {
            m_movingScenery = false;

            if (m_sceneryFrame.GetRotate())
                m_rotatingScenery = true;
            else if (m_sceneryFrame.GetScale())
                m_scalingScenery = true;
        }

        if (!m_movingScenery && !m_rotatingScenery && !m_scalingScenery)
        {
            m_sceneryFrame.AddScenery(m_currentSceneryTypeName, m_sceneryId, m_newScenery.style);
            m_sceneryId = m_canvas.AddScenery(m_newScenery);
            m_movingScenery = true;
            m_newScenery.rotation = 0;
            m_newScenery.scaleX = 1.0;
            m_newScenery.scaleY = 1.0;
        }
    }
}

void SceneryTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
}

void SceneryTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    if (m_editingScenery)
    {
        auto positionOnMap = m_canvas.GetMousePositionOnMap(
            { event.GetPosition().x, event.GetPosition().y });

        if (m_movingScenery)
        {
            m_newScenery.x = positionOnMap.x;
            m_newScenery.y = positionOnMap.y;
        }
        
        if (m_rotatingScenery)
        {
            double angle = atan2f(m_newScenery.y - positionOnMap.y, positionOnMap.x - m_newScenery.x);
            m_newScenery.rotation = angle;
        }

        if (m_scalingScenery)
        {
            float distanceX = (positionOnMap.x - m_newScenery.x);
            float distanceY = (positionOnMap.y - m_newScenery.y);

            float angle = -m_newScenery.rotation;
            float newWidth = distanceX * cos(angle) + distanceY * sin(angle);
            float newHeight = distanceY * cos(angle) - distanceX * sin(angle);

            m_newScenery.scaleX = newWidth / m_newScenery.width;
            m_newScenery.scaleY = newHeight / m_newScenery.height;
        }

        m_newScenery.level = m_sceneryFrame.GetLevel();

        m_newScenery.alpha = m_palette.GetColor().Alpha();
        m_newScenery.color.alpha = m_palette.GetColor().Alpha();
        m_newScenery.color.red = m_palette.GetColor().Red();
        m_newScenery.color.green = m_palette.GetColor().Green();
        m_newScenery.color.blue = m_palette.GetColor().Blue();

        m_canvas.EditScenery(m_sceneryId, m_newScenery);
        m_canvas.Refresh();
    }
}

void SceneryTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
    wxFileDialog pickSceneryDialog(&m_canvas, wxFileSelectorPromptStr,
        m_settings.GetSoldatPath() + "scenery-gfx/", wxEmptyString,
        "All files (*.*)|*.*", wxFD_OPEN);

    if (pickSceneryDialog.ShowModal() != wxID_CANCEL)
    {
        if (m_editingScenery)
        {
            m_canvas.RemoveScenery(m_sceneryId);
        }

        auto positionOnMap = m_canvas.GetMousePositionOnMap(
            { event.GetPosition().x, event.GetPosition().y });

        Image sceneryImage;
        sceneryImage.OpenAndResize(pickSceneryDialog.GetPath());

        m_newScenery.active = true;
        m_newScenery.alpha = m_palette.GetColor().Alpha();
        m_newScenery.color.alpha = m_palette.GetColor().Alpha();
        m_newScenery.color.red = m_palette.GetColor().Red();
        m_newScenery.color.green = m_palette.GetColor().Green();
        m_newScenery.color.blue = m_palette.GetColor().Blue();
        m_newScenery.level = m_sceneryFrame.GetLevel();
        m_newScenery.rotation = 0;
        m_newScenery.scaleX = 1.0;
        m_newScenery.scaleY = 1.0;
        m_newScenery.width = sceneryImage.GetWidth();
        m_newScenery.height = sceneryImage.GetHeight();
        m_newScenery.x = positionOnMap.x;
        m_newScenery.y = positionOnMap.y;

        m_newScenery.style = m_canvas.AddSceneryType(pickSceneryDialog.GetFilename());
        m_currentSceneryTypeName = pickSceneryDialog.GetFilename();

        m_sceneryId = m_canvas.AddScenery(m_newScenery);
        m_editingScenery = true;
        m_movingScenery = true;
        m_rotatingScenery = false;
        m_scalingScenery = false;

        m_canvas.Refresh();
    }
}

void SceneryTool::OnCanvasKeyPress(const wxKeyEvent &event)
{
}

void SceneryTool::OnTimerTick()
{
}

void SceneryTool::OnSceneryFrameSelect(wxListEvent& event)
{
    if (m_toolSelected)
    {
        if (m_editingScenery)
        {
            m_canvas.RemoveScenery(m_sceneryId);
        }

        const wxPoint mousePosition = wxGetMousePosition();
        auto positionOnMap = m_canvas.GetMousePositionOnMap({ mousePosition.x, mousePosition.y });

        m_newScenery.active = true;
        m_newScenery.alpha = m_palette.GetColor().Alpha();
        m_newScenery.color.alpha = m_palette.GetColor().Alpha();
        m_newScenery.color.red = m_palette.GetColor().Red();
        m_newScenery.color.green = m_palette.GetColor().Green();
        m_newScenery.color.blue = m_palette.GetColor().Blue();
        m_newScenery.level = m_sceneryFrame.GetLevel();
        m_newScenery.rotation = 0;
        m_newScenery.scaleX = 1.0;
        m_newScenery.scaleY = 1.0;
        m_newScenery.x = positionOnMap.x;
        m_newScenery.y = positionOnMap.y;

        Image sceneryImage;
        sceneryImage.OpenAndResize(m_settings.GetSoldatPath() + "scenery-gfx/" + event.GetItem().GetText());

        m_newScenery.width = sceneryImage.GetWidth();
        m_newScenery.height = sceneryImage.GetHeight();
        m_newScenery.style = event.GetItem().GetData();

        m_sceneryId = m_canvas.AddScenery(m_newScenery);

        m_editingScenery = true;
        m_movingScenery = true;
        m_rotatingScenery = false;
        m_scalingScenery = false;

        m_canvas.Refresh();
    }

    event.Skip();
}
