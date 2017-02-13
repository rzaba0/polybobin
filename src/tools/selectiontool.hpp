#pragma once
#include "tool.hpp"

class SelectionTool : public Tool
{
public:
    using Tool::Tool;
    void OnSelect() override;
    void OnUnselect() override;
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
    void OnCanvasMouseMotion(const wxMouseEvent &event) override;
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
    void OnTimerTick() override;
    ~SelectionTool() override = default;
};