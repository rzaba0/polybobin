#include "polygontool.hpp"

PolygonTool::PolygonTool(GLCanvas& canvas, const PaletteFrame& palette)
    : m_canvas{canvas}
    , m_palette{palette}
    , m_addedPolygonVerticesCount{}
    , m_newPolygonType{PMSPolygonType::ptNORMAL}
    , m_polygonId{}
{
}

void PolygonTool::OnSelect()
{

}

void PolygonTool::OnUnselect()
{

}

void PolygonTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    // TODO: separate this functionality somehow
    PMSVertex vertex = m_canvas.CreateVertex(m_palette.GetColor(), event.GetPosition());
    if (m_addedPolygonVerticesCount == 0)
    {
        PMSVector perpendicular;
        perpendicular.x = 0.0f;
        perpendicular.y = 0.0f;
        perpendicular.z = 2.0f;
        PMSPolygon polygon;
        for (unsigned i = 0; i < 3; ++i)
        {
            polygon.vertices[i] = vertex;
            polygon.perpendiculars[i] = perpendicular;
        }
        polygon.polygonType = m_newPolygonType;
        m_polygonId = m_canvas.AddPolygon(polygon, vertex);
    }
    else
    {
        unsigned vertexIndex = m_addedPolygonVerticesCount;
        m_canvas.EditPolygonVertex(m_polygonId, m_newPolygonType, vertexIndex, vertex);
    }

    ++m_addedPolygonVerticesCount;
    if (m_addedPolygonVerticesCount == 3)
    {
        m_addedPolygonVerticesCount = 0;
    }

    m_canvas.Refresh();
}

void PolygonTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    if (m_addedPolygonVerticesCount)
    {
        PMSVertex vertex = m_canvas.CreateVertex(m_palette.GetColor(), event.GetPosition());

        // Update the positions of the vertices that haven't been set yet.
        m_canvas.EditPolygonVertex(m_polygonId, m_newPolygonType, m_addedPolygonVerticesCount, vertex);
        m_canvas.Refresh();
    }
}

void PolygonTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) 
{
    wxMenu *newPolygonTypeSelection = new wxMenu();
    for (unsigned int i = 0; i < POLYGON_TYPES_COUNT; ++i)
    {
        newPolygonTypeSelection->AppendCheckItem(ID_POLYGON_TYPE_NORMAL + i, POLYGON_TYPES_NAMES[i]);
    }
    newPolygonTypeSelection->Bind(wxEVT_MENU, &PolygonTool::OnNewPolygonTypeSelected, this);
    newPolygonTypeSelection->Check(ID_POLYGON_TYPE_NORMAL + (int)m_newPolygonType, true);
    m_canvas.PopupMenu(newPolygonTypeSelection);
}

void PolygonTool::OnTimerTick()
{

}

// TODO: allow to select a new type while creating polygon?
void PolygonTool::OnNewPolygonTypeSelected(wxCommandEvent &event)
{
    if (!m_addedPolygonVerticesCount)
    {
        int selectedPolygonType = event.GetId() - ID_POLYGON_TYPE_NORMAL;
        m_newPolygonType = (PMSPolygonType)selectedPolygonType;
    }
}