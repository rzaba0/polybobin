#include "spawnpointtool.hpp"

SpawnPointTool::SpawnPointTool(GLCanvas &canvas)
    : m_canvas{canvas}
    , m_spawnPointIdx{}
    , m_spawnPointType{PMSSpawnPointType::sptALPHA}
{
}

void SpawnPointTool::OnSelect()
{
    PMSSpawnPoint spawnPoint(0, 0, m_spawnPointType);
    m_spawnPointIdx = m_canvas.AddSpawnPoint(spawnPoint);
}

void SpawnPointTool::OnUnselect()
{
    wxVector<unsigned int> spawnPointIdx;
    spawnPointIdx.push_back((unsigned int) m_spawnPointIdx);
    m_canvas.RemoveSpawnPoints(spawnPointIdx);
    m_canvas.Refresh();
}

void SpawnPointTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    OnSelect();
}

void SpawnPointTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
}

void SpawnPointTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
    PMSSpawnPoint spawnPoint(m_mousePositionOnMap.x, m_mousePositionOnMap.y, m_spawnPointType);
    m_canvas.EditSpawnPoint(m_spawnPointIdx, spawnPoint);
    m_canvas.Refresh();
}

void SpawnPointTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
    wxMenu *newSpawnPointTypeSelection = new wxMenu();
    for (unsigned int i = 0; i < SPAWNPOINT_TYPES_COUNT; ++i)
    {
        newSpawnPointTypeSelection->AppendCheckItem(ID_SPAWNPOINT_TYPE_GENERAL + i, SPAWNPOINT_TYPES_NAMES[i]);
    }
    newSpawnPointTypeSelection->Bind(wxEVT_MENU, &SpawnPointTool::OnNewSpawnPointTypeSelected, this);
    newSpawnPointTypeSelection->Check(ID_SPAWNPOINT_TYPE_GENERAL + (int)m_spawnPointType, true);
    m_canvas.PopupMenu(newSpawnPointTypeSelection);
}

void SpawnPointTool::OnTimerTick()
{
}

void SpawnPointTool::OnNewSpawnPointTypeSelected(wxCommandEvent &event)
{
    int selectedSpawnPointType = event.GetId() - ID_SPAWNPOINT_TYPE_GENERAL;
    m_spawnPointType = (PMSSpawnPointType)selectedSpawnPointType;
    
    PMSSpawnPoint spawnPoint(m_mousePositionOnMap.x, m_mousePositionOnMap.y, m_spawnPointType);
    m_canvas.EditSpawnPoint(m_spawnPointIdx, spawnPoint);
    m_canvas.Refresh();
}
