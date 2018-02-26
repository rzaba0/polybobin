#include "vertexselectiontool.hpp"

VertexSelectionTool::VertexSelectionTool(SelectionManager& selectionManager, Canvas& canvas)
    : m_selectionManager{selectionManager}
    , m_canvas{canvas}
    , m_leftDown{false}
{
}

void VertexSelectionTool::OnSelect()
{
    m_mousePositionOnMap = {};
    m_leftDown = false;
}

void VertexSelectionTool::OnUnselect()
{
}

void VertexSelectionTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
    m_selectionBeginPosition = m_mousePositionOnMap;
    m_leftDown = true;
}

void VertexSelectionTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
    if (wxGetKeyState(MOVE_SELECTED_KEY)) return;
    if (m_leftDown)
    {
        m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
        if (wxGetKeyState(REMOVE_SELECTION_KEY))
        {
            m_selectionManager.RectangularUnselect(m_selectionBeginPosition, m_mousePositionOnMap);
        }
        else
            m_selectionManager.RectangularSelect(m_selectionBeginPosition, m_mousePositionOnMap, wxGetKeyState(ADD_SELECTION_KEY));
    }
}

void VertexSelectionTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    // FIXME: duplicate code from selectiontool
    wxRealPoint newMousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
    if (event.LeftIsDown() && wxGetKeyState(MOVE_SELECTED_KEY))
    {
        float dx = newMousePositionOnMap.x - m_mousePositionOnMap.x;
        float dy = newMousePositionOnMap.y - m_mousePositionOnMap.y;
        m_selectionManager.MoveSelection(dx, dy);
    }
    m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
}

void VertexSelectionTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
}

void VertexSelectionTool::OnCanvasKeyPress(const wxKeyEvent &event)
{
}

void VertexSelectionTool::OnTimerTick()
{
}