#include "scenerytool.hpp"
#include "../gl/image.hpp"

SceneryTool::SceneryTool(GLCanvas& canvas, const Settings& settings, const PaletteFrame& palette)
    : m_canvas(canvas)
    , m_settings(settings)
    , m_palette(palette)
    , m_movingScenery(false)
    , m_sceneryId(0)
{
}

void SceneryTool::OnSelect()
{
    m_movingScenery = false;
}

void SceneryTool::OnUnselect()
{
    if (m_movingScenery)
    {
        m_canvas.RemoveScenery(m_sceneryId);
        m_canvas.Refresh();
    }

    m_movingScenery = false;
}

void SceneryTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    if (m_movingScenery)
    {
        m_sceneryId = m_canvas.AddScenery(m_newScenery);
    }
}

void SceneryTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
}

void SceneryTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    if (m_movingScenery)
    {
        auto positionOnMap = m_canvas.GetMousePositionOnMap(
            { event.GetPosition().x, event.GetPosition().y });

        m_newScenery.x = positionOnMap.x;
        m_newScenery.y = positionOnMap.y;
        // TODO: set rotation and scale
        //m_newScenery.rotation = 0;
        //m_newScenery.scaleX = 1.0;
        //m_newScenery.scaleY = 1.0;

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
        m_settings.GetSoldatPath() + "scenery-gfx\\", wxEmptyString,
        "All files (*.*)|*.*", wxFD_OPEN);

    if (pickSceneryDialog.ShowModal() != wxID_CANCEL)
    {
        if (m_movingScenery)
        {
            m_canvas.RemoveScenery(m_sceneryId);
        }

        auto positionOnMap = m_canvas.GetMousePositionOnMap(
            { event.GetPosition().x, event.GetPosition().y });

        Image tmpImage;
        tmpImage.OpenAndResize(pickSceneryDialog.GetPath());

        m_newScenery.active = true;
        m_newScenery.alpha = m_palette.GetColor().Alpha();
        m_newScenery.color.alpha = m_palette.GetColor().Alpha();
        m_newScenery.color.red = m_palette.GetColor().Red();
        m_newScenery.color.green = m_palette.GetColor().Green();
        m_newScenery.color.blue = m_palette.GetColor().Blue();
        m_newScenery.level = lBEHIND_NONE;
        m_newScenery.rotation = 0;
        m_newScenery.scaleX = 1.0;
        m_newScenery.scaleY = 1.0;
        m_newScenery.width = tmpImage.GetWidth();
        m_newScenery.height = tmpImage.GetHeight();
        m_newScenery.x = positionOnMap.x;
        m_newScenery.y = positionOnMap.y;

        m_newScenery.style = m_canvas.AddSceneryType(pickSceneryDialog.GetFilename());

        m_sceneryId = m_canvas.AddScenery(m_newScenery);
        m_movingScenery = true;

        m_canvas.Refresh();
    }
}

void SceneryTool::OnCanvasKeyPress(const wxKeyEvent &event)
{
}

void SceneryTool::OnTimerTick()
{
}