#include "selectionmanager.hpp"
#include <utility>

SelectionManager::SelectionManager(Canvas& canvas, const DisplaySettings& displaySettings, std::unique_ptr<PolygonSelection> polygonSelection, std::unique_ptr<Selection> scenerySelection)
    : m_canvas{canvas}
    , m_displaySettings{displaySettings}
    , m_polygonSelection{std::move(polygonSelection)}
    , m_scenerySelection{std::move(scenerySelection)}
{
}

void SelectionManager::SelectAll()
{
    if (m_displaySettings.ShouldDisplayPolygons())
        m_polygonSelection->selectAll(m_canvas.GetPolygonCount());
}

void SelectionManager::UnselectAll()
{
    m_polygonSelection->unselectAll();
}

void SelectionManager::PunctualSelect(wxPoint p, bool append)
{
    if (!append) m_polygonSelection->unselectAll();
    unsigned polyNum = m_canvas.GetPolygonCount();
    for (unsigned i = 0; i < polyNum; ++i)
    {
        if (m_canvas.GetPolygon(i).Contains(p.x, p.y)) {
            m_polygonSelection->select(i);
        }
    }
}

void SelectionManager::RectangularSelect(wxPoint a, wxPoint b, bool append)
{

}

void SelectionManager::PunctualUnselect(wxPoint p)
{
    unsigned polyNum = m_canvas.GetPolygonCount();
    for (unsigned i = 0; i < polyNum; ++i)
    {
        if (m_canvas.GetPolygon(i).Contains(p.x, p.y)) {
            m_polygonSelection->unselect(i);
        }
    }
}

void SelectionManager::RectangularUnselect(wxPoint a, wxPoint b)
{

}

void SelectionManager::CompletePolygonSelection()
{
    m_polygonSelection->completeSelection();
}

void SelectionManager::MoveSelection(float vx, float vy)
{
    unsigned polyNum = m_canvas.GetPolygonCount();
    for (unsigned i = 0; i < polyNum; ++i)
    {
    }
}