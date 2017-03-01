#pragma once
#include "tool.hpp"
#include "../canvas.hpp"
#include "../selectionmanager.hpp"

class VertexSelectionTool : public Tool
{
public:
    VertexSelectionTool(SelectionManager& selectionManager, Canvas& canvas);
    void OnSelect() override;
    void OnUnselect() override;
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
    void OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasMouseMotion(const wxMouseEvent &event) override;
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
    void OnTimerTick() override;
    ~VertexSelectionTool() override = default;

private:
    SelectionManager& m_selectionManager;
    Canvas& m_canvas;
    wxRealPoint m_mousePositionOnMap;
    wxRealPoint m_selectionBeginPosition;
    bool m_leftDown;
};