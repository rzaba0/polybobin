#include "texturetool.hpp"

TextureTool::TextureTool(SelectionManager& selectionManager, Canvas& glCanvas)
    : m_selectionManager(selectionManager)
    , m_canvas(glCanvas)
{
}

void TextureTool::OnSelect()
{
    m_canvas.SetFillPolygonSelection(true);
}

void TextureTool::OnUnselect()
{
    m_canvas.SetFillPolygonSelection(false);
}

void TextureTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
}

void TextureTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
}

void TextureTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    wxRealPoint newMousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
    if (event.LeftIsDown())
    {
        float dx = newMousePositionOnMap.x - m_mousePositionOnMap.x;
        float dy = newMousePositionOnMap.y - m_mousePositionOnMap.y;
        dx /= 128.;
        dy /= 128.;
        m_selectionManager.MoveSelectedTextures(-dx, -dy);
    }
    m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
}

void TextureTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
}

void TextureTool::OnCanvasKeyPress(const wxKeyEvent &event)
{
}

void TextureTool::OnTimerTick()
{
}
