#include "selectiontool.hpp"

void SelectionTool::OnSelect()
{

}

void SelectionTool::OnUnselect()
{

}

void SelectionTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    const auto &polygons = m_map.GetPolygons();
    const auto &scenery = m_map.GetSceneryInstances();

    bool addSelectionKeyPressed = wxGetKeyState(ADD_SELECTION_KEY),
        removeSelectionKeyPressed = wxGetKeyState(REMOVE_SELECTION_KEY),
        moveSelectedKeyPressed = wxGetKeyState(MOVE_SELECTED_KEY);
    bool skippedPolygon = false,
        skippedScenery = false;
    unsigned int skippedPolygonId,
        skippedSceneryId;

    if (moveSelectedKeyPressed)
    {
        m_movingSelected = true;
        return;
    }

    for (size_t i = 0; i < polygons.size(); ++i)
    {
        if (polygons[i].Contains((float)m_mousePositionOnMap.x, (float)m_mousePositionOnMap.y))
        {
            if (m_selectedPolygons.IsSelected(i))
            {
                if (removeSelectionKeyPressed)
                {
                    m_selectedPolygons.Unselect(i);
                    Refresh();
                    return;
                }
                skippedPolygon = true;
                skippedPolygonId = i;
            }
            else
            {
                if (!addSelectionKeyPressed && !removeSelectionKeyPressed)
                {
                    m_selectedPolygons.UnselectAll();
                    m_selectedScenery.UnselectAll();
                }

                if (!removeSelectionKeyPressed)
                {
                    m_selectedPolygons.Select(i);
                }

                Refresh();
                return;
            }
        }
    }

    for (size_t i = 0; i < scenery.size(); ++i)
    {
        if (scenery[i].Contains((float)m_mousePositionOnMap.x, (float)m_mousePositionOnMap.y))
        {
            if (m_selectedScenery.IsSelected(i))
            {
                if (removeSelectionKeyPressed)
                {
                    m_selectedScenery.Unselect(i);
                    Refresh();
                    return;
                }
                skippedScenery = true;
                skippedSceneryId = i;
            }
            else
            {
                if (!addSelectionKeyPressed && !removeSelectionKeyPressed)
                {
                    m_selectedPolygons.UnselectAll();
                    m_selectedScenery.UnselectAll();
                }

                if (!removeSelectionKeyPressed)
                {
                    m_selectedScenery.Select(i);
                }

                Refresh();
                return;
            }
        }
    }

    if (!addSelectionKeyPressed && !removeSelectionKeyPressed)
    {
        m_selectedPolygons.UnselectAll();
        m_selectedScenery.UnselectAll();
    }

    if (!skippedPolygon && skippedScenery && !addSelectionKeyPressed)
    {
        m_selectedScenery.Select(skippedSceneryId);
    }

    if (skippedPolygon && !skippedScenery && !addSelectionKeyPressed)
    {
        m_selectedPolygons.Select(skippedPolygonId);
    }
    Refresh();
}

void SelectionTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
   
}

void SelectionTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) 
{

}

void SelectionTool::OnTimerTick()
{

}