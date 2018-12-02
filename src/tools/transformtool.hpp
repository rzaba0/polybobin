#pragma once

#include "tool.hpp"
#include "../selectionmanager.hpp"
#include "../canvas.hpp"

class TransformTool : public Tool
{
public:
    TransformTool(SelectionManager& selectionManager, Canvas &canvas);
    void OnSelect() override;
    void OnUnselect() override;
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
    void OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasMouseMotion(const wxMouseEvent &event) override;
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasKeyPress(const wxKeyEvent &event) override;
    void OnTimerTick() override;
    ~TransformTool() override = default;

private:
    void SetupOrigins();
    float Distance(const wxRealPoint &p1, const wxRealPoint &p2);
    float Angle(const wxRealPoint &p1, const wxRealPoint &p2);
    wxRealPoint Rotate(const wxRealPoint &origin, const wxRealPoint &p, float angle);
    wxRealPoint Scale(const wxRealPoint &origin, const wxRealPoint &p, const wxRealPoint &scale);

    SelectionManager& m_selectionManager;
    Canvas &m_canvas;

    wxRealPoint m_mousePositionOnMap;

    wxRealPoint m_bottomLeft;
    wxRealPoint m_bottomRight;
    wxRealPoint m_topLeft;
    wxRealPoint m_topRight;

    bool m_moving;
    bool m_scaling;
    bool m_rotating;
    bool m_temporarySelection;

    wxRealPoint m_origin;
    wxRealPoint m_current;
};

