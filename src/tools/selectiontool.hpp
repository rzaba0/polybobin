#pragma once
#include "tool.hpp"
#include "../canvas.hpp"
#include "../selectionmanager.hpp"

class SelectionTool : public Tool
{
public:
    SelectionTool(SelectionManager& selectionManager, Canvas& glCanvas);
    void OnSelect() override;
    void OnUnselect() override;
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
    void OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasMouseMotion(const wxMouseEvent &event) override;
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
    void OnTimerTick() override;
    ~SelectionTool() override = default;

private:
    SelectionManager& m_selectionManager;
    Canvas& m_canvas;
    wxRealPoint m_mousePositionOnMap;
};