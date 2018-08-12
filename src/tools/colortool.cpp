#include "colortool.hpp"

ColorTool::ColorTool(GLCanvas& canvas, 
        SelectionManager& selectionManager,
        const PaletteFrame& palette)
    : m_canvas(canvas)
    , m_selectionManager(selectionManager)
    , m_palette(palette)
{
}

void ColorTool::OnSelect()
{
    m_canvas.SetFillPolygonSelection(true);
}

void ColorTool::OnUnselect()
{
    m_canvas.SetFillPolygonSelection(false);
}

void ColorTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    if (m_selectionManager.Empty())
    {
        auto positionOnMap = m_canvas.GetMousePositionOnMap(
                { event.GetPosition().x, event.GetPosition().y });

        for (auto i = 0; i < m_canvas.GetPolygonCount(); i++)
        {
            auto polygon = m_canvas.GetPolygon(i);
            if (polygon.Contains(positionOnMap.x, positionOnMap.y))
            {
                for (auto j = 0; j < 3; j++)
                {
                    polygon.vertices[j].color = m_palette.GetColor();
                    m_canvas.EditPolygonVertex(i, polygon.polygonType,
                            j, polygon.vertices[j]);
                }
            }
        }

        for (auto i = 0; i < m_canvas.GetSceneryCount(); i++)
        {
            auto scenery = m_canvas.GetScenery(i);
            if (scenery.Contains(positionOnMap.x, positionOnMap.y))
            {
                scenery.color = m_palette.GetColor();
                m_canvas.EditScenery(i, scenery);
            }
        }
    }
    else
    {
        for (const auto poly : *m_selectionManager.GetPolygonSelection())
        {
            auto polygon = m_canvas.GetPolygon(poly.id);
            for (int i = 0; i < 3; i++)
            {
                if (poly.vertex[i])
                {
                    polygon.vertices[i].color = m_palette.GetColor();
                    m_canvas.EditPolygonVertex(poly.id, polygon.polygonType,
                            i, polygon.vertices[i]);
                }
            }
        }

        for (auto sceneryId : *m_selectionManager.GetScenerySelection())
        {
            auto scenery = m_canvas.GetScenery(sceneryId);
            scenery.color = m_palette.GetColor();
            m_canvas.EditScenery(sceneryId, scenery);
        }
    }

    m_canvas.Draw();
}

void ColorTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
}

void ColorTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
}

void ColorTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
}

void ColorTool::OnCanvasKeyPress(const wxKeyEvent &event)
{
}

void ColorTool::OnTimerTick()
{
}
