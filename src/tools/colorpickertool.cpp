#include "colorpickertool.hpp"

#include "../utils.hpp"

ColorPickerTool::ColorPickerTool(GLCanvas& canvas,
            SelectionManager& selectionManager,
            PaletteFrame& palette)
    : m_canvas(canvas)
    , m_selectionManager(selectionManager)
    , m_palette(palette)
{
}

void ColorPickerTool::OnSelect()
{
}

void ColorPickerTool::OnUnselect()
{
}

void ColorPickerTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    bool found = false;
    auto positionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
    wxColor color;

    if (wxGetKeyState(COLOR_PICKER_PIXEL_KEY))
    {
        // TODO: this should return pixel color at mouse position
    }
    else
    {
        unsigned polyCount = m_canvas.GetPolygonCount();
        unsigned sceneryCount = m_canvas.GetSceneryCount();

        for (unsigned sceneryId = 0; sceneryId < sceneryCount; sceneryId++)
        {
            auto scenery = m_canvas.GetScenery(sceneryId);

            if (scenery.level != lBEHIND_ALL)
                continue;

            if (scenery.Contains(positionOnMap.x, positionOnMap.y))
            {
                color = wxColor(scenery.color.red, scenery.color.green, scenery.color.blue, scenery.color.alpha);
                found = true;
            }
        }

        for (unsigned sceneryId = 0; sceneryId < sceneryCount; sceneryId++)
        {
            auto scenery = m_canvas.GetScenery(sceneryId);

            if (scenery.level != lBEHIND_MAP)
                continue;

            if (scenery.Contains(positionOnMap.x, positionOnMap.y))
            {
                color = wxColor(scenery.color.red, scenery.color.green, scenery.color.blue, scenery.color.alpha);
                found = true;
            }
        }

        for (unsigned polyId = 0; polyId < polyCount; polyId++)
        {
            auto poly = m_canvas.GetPolygon(polyId);

            if (poly.Contains(positionOnMap.x, positionOnMap.y))
            {
                float d[3];
                for (unsigned i = 0; i < 3; i++)
                {
                    d[i] = Utils::Distance(positionOnMap.x, positionOnMap.y, poly.vertices[i].x, poly.vertices[i].y);
                }

                PMSColor polyColor;
                if (d[0] < d[1] && d[0] < d[2])
                {
                    polyColor = poly.vertices[0].color;
                }
                else if (d[1] < d[0] && d[1] < d[2])
                {
                    polyColor = poly.vertices[1].color;
                }
                else
                {
                    polyColor = poly.vertices[2].color;
                }

                color = wxColor(polyColor.red, polyColor.green, polyColor.blue, polyColor.alpha);
                found = true;
            }
        }

        for (unsigned sceneryId = 0; sceneryId < sceneryCount; sceneryId++)
        {
            auto scenery = m_canvas.GetScenery(sceneryId);

            if (scenery.level != lBEHIND_NONE)
                continue;

            if (scenery.Contains(positionOnMap.x, positionOnMap.y))
            {
                color = wxColor(scenery.color.red, scenery.color.green, scenery.color.blue, scenery.color.alpha);
                found = true;
            }
        }
    }

    if (found)
        m_palette.SetColor(color);
}

void ColorPickerTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
}

void ColorPickerTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
}

void ColorPickerTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
}

void ColorPickerTool::OnCanvasKeyPress(const wxKeyEvent &event)
{
}

void ColorPickerTool::OnTimerTick()
{
}
