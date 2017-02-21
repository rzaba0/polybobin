#include "selectiontool.hpp"
#include "../constants.hpp"

SelectionTool::SelectionTool(SelectionManager& selectionManager, GLCanvas& glCanvas)
    : m_selectionManager{selectionManager}
    , m_canvas{glCanvas}
{
}

void SelectionTool::OnSelect()
{
    m_selectionManager.CompletePolygonSelection();
}

void SelectionTool::OnUnselect()
{
}

void SelectionTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    auto positionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
    if (wxGetKeyState(MOVE_SELECTED_KEY)) return;
    if (wxGetKeyState(REMOVE_SELECTION_KEY))
    {
        m_selectionManager.PunctualUnselect(positionOnMap);
    }
    else
        m_selectionManager.PunctualSelect(positionOnMap, wxGetKeyState(ADD_SELECTION_KEY));
    m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
}

void SelectionTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
}

void SelectionTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    wxRealPoint newMousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
    if (event.LeftIsDown() && wxGetKeyState(MOVE_SELECTED_KEY))
    {
        float dx = newMousePositionOnMap.x - m_mousePositionOnMap.x;
        float dy = newMousePositionOnMap.y - m_mousePositionOnMap.y;
        m_selectionManager.MoveSelection(dx, dy);
    }
    m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
}

void SelectionTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) 
{
}

void SelectionTool::OnTimerTick()
{
}