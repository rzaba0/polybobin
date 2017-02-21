#pragma once
#include <vector>
#include <memory>
#include "tools/tool.hpp"
#include "canvas.hpp"

using ToolSet = std::vector<std::unique_ptr<Tool>>;

class EventDispatcher
{
public:
    EventDispatcher(ToolSet tools, Canvas& canvas);
    void Select(int toolId);
    void Unselect();
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event);
    void OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event);
    void OnCanvasMouseMotion(const wxMouseEvent &event);
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event);
    ~EventDispatcher();
    
private:
    ToolSet m_tools;
    Canvas& m_canvas;
    int m_selectedToolId;
};