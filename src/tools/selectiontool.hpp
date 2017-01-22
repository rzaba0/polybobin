#pragma once
#include "tool.hpp"

class SelectionTool : public Tool
{
public:
    SelectionTool(GLCanvas& canvas);
    void OnSelect() override;
    void OnUnselect() override;
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
    void OnCanvasMouseMotion(const wxMouseEvent &event) override;
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
};