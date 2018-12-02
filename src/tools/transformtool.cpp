#include "transformtool.hpp"

#include <cmath>

#include "../constants.hpp"

TransformTool::TransformTool(SelectionManager& selectionManager, Canvas &canvas)
    : m_selectionManager(selectionManager)
    , m_canvas(canvas)
    , m_bottomLeft(wxRealPoint(0.0, 0.0))
    , m_bottomRight(wxRealPoint(0.0, 0.0))
    , m_topLeft(wxRealPoint(0.0, 0.0))
    , m_topRight(wxRealPoint(0.0, 0.0))
    , m_moving(false)
    , m_scaling(false)
    , m_rotating(false)
    , m_temporarySelection(false)
    , m_origin(wxRealPoint(0.0, 0.0))
    , m_current(wxRealPoint(0.0, 0.0))
{
}

void TransformTool::OnSelect()
{
    SetupOrigins();
}

void TransformTool::OnUnselect()
{
    if (m_temporarySelection)
    {
        m_selectionManager.UnselectAll();
    }
    m_canvas.SetTransformFrameVisible(false);
}

void TransformTool::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    if (m_selectionManager.Empty())
    {
        auto positionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());

        m_selectionManager.PunctualSelect(positionOnMap, wxGetKeyState(ADD_SELECTION_KEY));

        if (!m_selectionManager.Empty())
        {
            m_moving = true;
            m_temporarySelection = true;
            SetupOrigins();
        }
    }
    else
    {
        m_rotating = wxGetKeyState(TRANSFORM_ROTATE_KEY);
        m_scaling = wxGetKeyState(TRANSFORM_SCALE_KEY);
        m_moving = !m_rotating && !m_scaling;

        if (m_rotating || m_scaling)
        {
            wxRealPoint mousePosition = event.GetPosition();
            wxRealPoint bottomLeft = m_canvas.GetMapPositionOnScreen(m_bottomLeft);
            wxRealPoint bottomRight = m_canvas.GetMapPositionOnScreen(m_bottomRight);
            wxRealPoint topLeft = m_canvas.GetMapPositionOnScreen(m_topLeft);
            wxRealPoint topRight = m_canvas.GetMapPositionOnScreen(m_topRight);

            bool originFound = false;

            if (Distance(mousePosition, bottomLeft) <= TRANSFORM_TOOL_ORIGIN_SIZE)
            {
                originFound = true;
                m_origin = m_topRight;
                m_current = m_bottomLeft;
            }
            else if (Distance(mousePosition, bottomRight) <= TRANSFORM_TOOL_ORIGIN_SIZE)
            {
                originFound = true;
                m_origin = m_topLeft;
                m_current = m_bottomRight;
            }
            else if (Distance(mousePosition, topLeft) <= TRANSFORM_TOOL_ORIGIN_SIZE)
            {
                originFound = true;
                m_origin = m_bottomRight;
                m_current = m_topLeft;
            }
            else if (Distance(mousePosition, topRight) <= TRANSFORM_TOOL_ORIGIN_SIZE)
            {
                originFound = true;
                m_origin = m_bottomLeft;
                m_current = m_topRight;
            }

            if (!originFound)
            {
                m_rotating = false;
                m_scaling = false;
            }
        }
    }
}

void TransformTool::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
    if (m_temporarySelection)
    {
        m_selectionManager.UnselectAll();
        m_canvas.SetTransformFrameVisible(false);
        m_temporarySelection = false;
    }

    m_rotating = false;
    m_scaling = false;
    m_moving = false;

    SetupOrigins();
}

void TransformTool::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    if (!m_selectionManager.Empty())
    {
        if (!wxGetKeyState(TRANSFORM_ROTATE_KEY))
            m_rotating = false;
        if (!wxGetKeyState(TRANSFORM_SCALE_KEY))
            m_scaling = false;

        wxRealPoint newMousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
        if (m_moving && event.LeftIsDown())
        {
            float dx = newMousePositionOnMap.x - m_mousePositionOnMap.x;
            float dy = newMousePositionOnMap.y - m_mousePositionOnMap.y;
            m_selectionManager.MoveSelection(dx, dy);

            m_bottomLeft += wxRealPoint(dx, dy);
            m_bottomRight += wxRealPoint(dx, dy);
            m_topLeft += wxRealPoint(dx, dy);
            m_topRight += wxRealPoint(dx, dy);
            m_canvas.SetTransformFramePosition(m_bottomLeft, m_bottomRight, m_topLeft, m_topRight);
        }
        else if (m_scaling && event.LeftIsDown())
        {
            float epsilon = EPSILON;
            wxRealPoint scale;
            if (abs(m_current.x - m_origin.x) <= epsilon)
                scale.x = 1;
            else
                scale.x = (newMousePositionOnMap.x - m_origin.x) / (m_current.x - m_origin.x);
            if (abs(m_current.y - m_origin.y) <= epsilon)
                scale.y = 1;
            else
                scale.y = (newMousePositionOnMap.y - m_origin.y) / (m_current.y - m_origin.y);

            // protection from getting transformed into line or dot
            if (abs(newMousePositionOnMap.x - m_origin.x) <= 1)
                scale.x = 1.0;
            if (abs(newMousePositionOnMap.y - m_origin.y) <= 0.1)
                scale.y = 1.0;

            m_current = Scale(m_origin, m_current, scale);
            m_bottomLeft = Scale(m_origin, m_bottomLeft, scale);
            m_bottomRight = Scale(m_origin, m_bottomRight, scale);
            m_topLeft = Scale(m_origin, m_topLeft, scale);
            m_topRight = Scale(m_origin, m_topRight, scale);
            m_canvas.SetTransformFramePosition(m_bottomLeft, m_bottomRight, m_topLeft, m_topRight);
            
            for (auto selectedPoly : *m_selectionManager.GetPolygonSelection())
            {
                auto poly = m_canvas.GetPolygon(selectedPoly.id);

                for (unsigned i = 0; i < 3; i++)
                {
                    auto vertex = poly.vertices[i];
                    wxRealPoint newPos = Scale(m_origin, {vertex.x, vertex.y}, scale);
                    vertex.x = newPos.x;
                    vertex.y = newPos.y;
                    
                    m_canvas.EditPolygonVertex(selectedPoly.id, poly.polygonType, i, vertex);
                }
            }

            for (auto sceneryId : *m_selectionManager.GetScenerySelection())
            {
                auto scenery = m_canvas.GetScenery(sceneryId);
                wxRealPoint newPos = Scale(m_origin, {scenery.x, scenery.y}, scale);
                float x2 = scenery.x + scenery.width * scenery.scaleX;
                float y2 = scenery.y + scenery.height * scenery.scaleY;
                scenery.x = newPos.x;
                scenery.y = newPos.y;
                wxRealPoint newPos2 = Scale(m_origin, {x2, y2}, scale);
                scenery.scaleX = (newPos2.x - scenery.x) / scenery.width;
                scenery.scaleY = (newPos2.y - scenery.y) / scenery.height;

                m_canvas.EditScenery(sceneryId, scenery);
            }

        }
        else if(m_rotating && event.LeftIsDown())
        {
            float angle = Angle(newMousePositionOnMap - m_origin, m_current - m_origin);

            m_current = Rotate(m_origin, m_current, angle);
            m_bottomLeft = Rotate(m_origin, m_bottomLeft, angle);
            m_bottomRight = Rotate(m_origin, m_bottomRight, angle);
            m_topLeft = Rotate(m_origin, m_topLeft, angle);
            m_topRight = Rotate(m_origin, m_topRight, angle);
            m_canvas.SetTransformFramePosition(m_bottomLeft, m_bottomRight, m_topLeft, m_topRight);
            
            for (auto selectedPoly : *m_selectionManager.GetPolygonSelection())
            {
                auto poly = m_canvas.GetPolygon(selectedPoly.id);

                for (unsigned i = 0; i < 3; i++)
                {
                    auto vertex = poly.vertices[i];
                    wxRealPoint newPos = Rotate(m_origin, {vertex.x, vertex.y}, angle);
                    vertex.x = newPos.x;
                    vertex.y = newPos.y;
                    
                    m_canvas.EditPolygonVertex(selectedPoly.id, poly.polygonType, i, vertex);
                }
            }

            for (auto sceneryId : *m_selectionManager.GetScenerySelection())
            {
                auto scenery = m_canvas.GetScenery(sceneryId);
                wxRealPoint newPos = Rotate(m_origin, {scenery.x, scenery.y}, angle);
                scenery.x = newPos.x;
                scenery.y = newPos.y;
                scenery.rotation -= angle;

                m_canvas.EditScenery(sceneryId, scenery);
            }
            m_canvas.Draw();
        }
    }
    m_mousePositionOnMap = m_canvas.GetMousePositionOnMap(event.GetPosition());
}

void TransformTool::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
}

void TransformTool::OnCanvasKeyPress(const wxKeyEvent &event)
{
}

void TransformTool::OnTimerTick()
{
}

void TransformTool::SetupOrigins()
{
    if (!m_selectionManager.Empty())
    {
        float x;
        float y;

        if (!m_selectionManager.GetPolygonSelection()->empty())
        {
            unsigned polyId = m_selectionManager.GetPolygonSelection()->begin()->id;
            x = m_canvas.GetPolygon(polyId).vertices[0].x;
            y = m_canvas.GetPolygon(polyId).vertices[0].y;
        }
        if (!m_selectionManager.GetScenerySelection()->empty())
        {
            unsigned sceneryId = *m_selectionManager.GetScenerySelection()->begin();
            x = m_canvas.GetScenery(sceneryId).x;
            y = m_canvas.GetScenery(sceneryId).y;
        }

        m_bottomLeft = {x, y};
        m_bottomRight = {x, y};
        m_topLeft = {x, y};
        m_topRight = {x, y};

        for (auto poly : *m_selectionManager.GetPolygonSelection())
        {
            for (unsigned i = 0; i < 3; i++)
            {
                auto vertice = m_canvas.GetPolygon(poly.id).vertices[i];

                m_bottomLeft.x = wxMin(m_bottomLeft.x, vertice.x);
                m_bottomLeft.y = wxMax(m_bottomLeft.y, vertice.y);
                m_bottomRight.x = wxMax(m_bottomRight.x, vertice.x);
                m_bottomRight.y = wxMax(m_bottomRight.y, vertice.y);
                m_topLeft.x = wxMin(m_topLeft.x, vertice.x);
                m_topLeft.y = wxMin(m_topLeft.y, vertice.y);
                m_topRight.x = wxMax(m_topRight.x, vertice.x);
                m_topRight.y = wxMin(m_topRight.y, vertice.y);
            }
        }

        for (auto sceneryId : *m_selectionManager.GetScenerySelection())
        {
            auto scenery = m_canvas.GetScenery(sceneryId);

            wxRealPoint positions[4];
            positions[0] = {scenery.x, scenery.y};
            positions[1] = Rotate(positions[0],
                                  {scenery.x + scenery.width * scenery.scaleX, scenery.y},
                                  -scenery.rotation);
            positions[2] = Rotate(positions[0],
                                  {scenery.x, scenery.y + scenery.height * scenery.scaleY},
                                  -scenery.rotation);
            positions[3] = Rotate(positions[0],
                                  {scenery.x + scenery.width * scenery.scaleX, scenery.y + scenery.height * scenery.scaleY},
                                  -scenery.rotation);

            for (unsigned i = 0; i < 4; i++)
            {
                m_bottomLeft.x = wxMin(m_bottomLeft.x, positions[i].x);
                m_bottomLeft.y = wxMax(m_bottomLeft.y, positions[i].y);
                m_bottomRight.x = wxMax(m_bottomRight.x, positions[i].x);
                m_bottomRight.y = wxMax(m_bottomRight.y, positions[i].y);
                m_topLeft.x = wxMin(m_topLeft.x, positions[i].x);
                m_topLeft.y = wxMin(m_topLeft.y, positions[i].y);
                m_topRight.x = wxMax(m_topRight.x, positions[i].x);
                m_topRight.y = wxMin(m_topRight.y, positions[i].y);
            }
        }


        m_canvas.SetTransformFramePosition(m_bottomLeft, m_bottomRight, m_topLeft, m_topRight);
        m_canvas.SetTransformFrameVisible();
    }
}

float TransformTool::Distance(const wxRealPoint &p1, const wxRealPoint &p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

float TransformTool::Angle(const wxRealPoint &p1, const wxRealPoint &p2)
{
    return atan2(p1.y, p1.x) - atan2(p2.y, p2.x);
}

wxRealPoint TransformTool::Rotate(const wxRealPoint &origin, const wxRealPoint &p, float angle)
{
    wxRealPoint result = p - origin;
    result = {cos(angle) * result.x - sin(angle) * result.y,
              sin(angle) * result.x + cos(angle) * result.y};
    result += origin;
    return result;
}

wxRealPoint TransformTool::Scale(const wxRealPoint &origin, const wxRealPoint &p, const wxRealPoint &scale)
{
    wxRealPoint result = p - origin;
    result.x *= scale.x;
    result.y *= scale.y;
    result += origin;
    return result;
}

