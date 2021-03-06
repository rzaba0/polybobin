#include "selectionmanager.hpp"
#include <utility>
#include <algorithm>

#include "utils.hpp"

SelectionManager::SelectionManager(Canvas& canvas, const DisplaySettings& displaySettings, SceneryFrame& sceneryFrame, std::unique_ptr<PolygonSelection> polygonSelection, std::unique_ptr<Selection> scenerySelection)
    : m_canvas{canvas}
    , m_displaySettings{displaySettings}
    , m_sceneryFrame{sceneryFrame}
    , m_polygonSelection{std::move(polygonSelection)}
    , m_scenerySelection{std::move(scenerySelection)}
{
}

void SelectionManager::SelectAll()
{
    if (m_displaySettings.ShouldDisplayPolygons())
        m_polygonSelection->selectAll(m_canvas.GetPolygonCount());
    if (m_displaySettings.ShouldDisplayScenery())
        m_scenerySelection->selectAll(m_canvas.GetSceneryCount());
    m_canvas.UpdatePolygonSelectionForRedraw();
    m_canvas.Draw();
}

void SelectionManager::UnselectAll()
{
    m_polygonSelection->unselectAll();
    m_scenerySelection->unselectAll();
    m_canvas.Draw();
}

void SelectionManager::PunctualSelect(wxRealPoint p, bool append)
{
    if (!append) m_polygonSelection->unselectAll();
    if (!append) m_scenerySelection->unselectAll();

    ForEachPolyAtPosition(p, [&](unsigned polyId) { m_polygonSelection->select(polyId); });
    ForEachSceneryAtPosition(p, [&](unsigned sceneryId) { m_scenerySelection->select(sceneryId); });

    m_canvas.UpdatePolygonSelectionForRedraw();
    m_canvas.Draw();
}

void SelectionManager::RectangularSelect(wxRealPoint a, wxRealPoint b, bool append)
{
    if (!append) m_polygonSelection->unselectAll();
    if (!append) m_scenerySelection->unselectAll();

    ForEachVerticleInRectangle(a, b, [&](unsigned polyId, unsigned vertexId) { m_polygonSelection->select(polyId, vertexId); });
    ForEachSceneryInRectangle(a, b, [&](unsigned sceneryId) { m_scenerySelection->select(sceneryId); });

    m_canvas.UpdatePolygonSelectionForRedraw();
    m_canvas.Draw();
}

void SelectionManager::PunctualUnselect(wxRealPoint p)
{
    ForEachPolyAtPosition(p, [&](unsigned polyId) { m_polygonSelection->unselect(polyId); });
    ForEachSceneryAtPosition(p, [&](unsigned sceneryId) { m_scenerySelection->unselect(sceneryId); });
    m_canvas.Draw();
}

void SelectionManager::RectangularUnselect(wxRealPoint a, wxRealPoint b)
{
    ForEachVerticleInRectangle(a, b, [&](unsigned polyId, unsigned vertexId) { m_polygonSelection->unselect(polyId, vertexId); });
    ForEachSceneryInRectangle(a, b, [&](unsigned sceneryId) { m_scenerySelection->unselect(sceneryId); });

    m_canvas.UpdatePolygonSelectionForRedraw();
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

    for (const auto& selectedSceneryId : *m_scenerySelection)
    {
        const auto& scenery = m_canvas.GetScenery(selectedSceneryId);
        PMSScenery newScenery = scenery;
        newScenery.x += vx;
        newScenery.y += vy;
        m_canvas.EditScenery(selectedSceneryId, newScenery);
    }
    m_canvas.Draw();
}

void SelectionManager::MoveSelectedTextures(float vx, float vy)
{
    for (const auto& selectedPolygon : *m_polygonSelection)
    {
        const auto& polygon = m_canvas.GetPolygon(selectedPolygon.id);
        for (unsigned i = 0; i < 3; i++)
        {
            if (selectedPolygon.vertex[i])
            {
                auto vertex = polygon.vertices[i];
                vertex.textureS += vx;
                vertex.textureT += vy;
                m_canvas.EditPolygonVertex(selectedPolygon.id, polygon.polygonType, i, std::move(vertex));
            }
        }
    }
    m_canvas.Draw();
}

void SelectionManager::RemoveSelection()
{
    wxVector<unsigned int> selectedPolygons;
    wxVector<unsigned int> selectedSceneries;
    for (const auto &selectedPolygon : *m_polygonSelection)
    {
        selectedPolygons.push_back(selectedPolygon.id);
    }

    for (const auto &selectedScenery : *m_scenerySelection)
    {
        selectedSceneries.push_back(selectedScenery);
        m_sceneryFrame.RemoveScenery(selectedScenery);
    }

    m_canvas.RemovePolygons(selectedPolygons);
    m_canvas.RemoveSceneries(selectedSceneries);

    UnselectAll();
    m_canvas.Draw();
}

bool SelectionManager::Empty() const
{
    return (m_polygonSelection->empty() && m_scenerySelection->empty());
}

void SelectionManager::ForEachPolyAtPosition(wxRealPoint p, std::function<void(unsigned)> operation)
{
    unsigned polyNum = m_canvas.GetPolygonCount();
    for (unsigned i = 0; i < polyNum; i++)
    {
        if (m_canvas.GetPolygon(i).Contains(p.x, p.y)) {
            operation(i);
        }
    }
}

void SelectionManager::ForEachVerticleInRectangle(wxRealPoint a, wxRealPoint b, std::function<void(unsigned, unsigned)> operation)
{
    float xMin = std::min(a.x, b.x);
    float xMax = std::max(a.x, b.x);
    float yMin = std::min(a.y, b.y);
    float yMax = std::max(a.y, b.y);
    unsigned polyNum = m_canvas.GetPolygonCount();
    for (unsigned i = 0; i < polyNum; i++)
    {
        const auto& polygon = m_canvas.GetPolygon(i);
        for (unsigned j = 0; j < 3; j++)
        {
            const auto& vertex = polygon.vertices[j];
            if (vertex.x >= xMin && vertex.x <= xMax && vertex.y >= yMin && vertex.y <= yMax)
            {
                operation(i, j);
            }
        }
    }
}

void SelectionManager::ForEachSceneryAtPosition(wxRealPoint position, std::function<void(unsigned)> operation)
{
    unsigned sceneryNum = m_canvas.GetSceneryCount();
    for (unsigned i = 0; i < sceneryNum; i++)
    {
        if (m_canvas.GetScenery(i).Contains(position.x, position.y)) {
            operation(i);
        }
    }
}

void SelectionManager::ForEachSceneryInRectangle(wxRealPoint a, wxRealPoint b, std::function<void(unsigned)> operation)
{
    unsigned sceneryNum = m_canvas.GetSceneryCount();
    for (unsigned i = 0; i < sceneryNum; i++)
    {
        const auto& scenery = m_canvas.GetScenery(i);
        if (Utils::RectanglesIntersect(a.x, a.y, b.x, b.y, scenery.x, scenery.y,
            scenery.x + scenery.width, scenery.y + scenery.height))
        {
            operation(i);
        }
    }
}
