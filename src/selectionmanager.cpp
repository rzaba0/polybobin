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
    m_canvas.UpdatePolygonSelectionForRedraw();
    m_canvas.Draw();
}

void SelectionManager::UnselectAll()
{
    m_polygonSelection->unselectAll();
    m_canvas.Draw();
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
    m_canvas.UpdatePolygonSelectionForRedraw();
    m_canvas.Draw();
}

void SelectionManager::RectangularSelect(wxPoint a, wxPoint b, bool append)
{
    if (!append) m_polygonSelection->unselectAll();
    m_canvas.UpdatePolygonSelectionForRedraw();
    m_canvas.Draw();
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
    m_canvas.Draw();
}

void SelectionManager::RectangularUnselect(wxPoint a, wxPoint b)
{
    m_canvas.Draw();
}

void SelectionManager::CompletePolygonSelection()
{
    m_polygonSelection->completeSelection();
    m_canvas.UpdatePolygonSelectionForRedraw();
    m_canvas.Draw();
}

void SelectionManager::MoveSelection(float vx, float vy)
{
    for (const auto& selectedPolygon : *m_polygonSelection)
    {
        const auto& polygon = m_canvas.GetPolygon(selectedPolygon.id);
        for (unsigned i = 0; i < 3; i++)
        {
            if (selectedPolygon.vertex[i])
            {
                auto vertex = polygon.vertices[i];
                vertex.x += vx;
                vertex.y += vy;
                m_canvas.EditPolygonVertex(selectedPolygon.id, polygon.polygonType, i, std::move(vertex));
            }
        }
    }
    m_canvas.Draw();
}