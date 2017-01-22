#include "eventdispatcher.hpp"

EventDispatcher::EventDispatcher(ToolSet tools)
    : selectedToolId{-1}
    , tools{std::move(tools)}
{
}

EventDispatcher::~EventDispatcher()
{
    Unselect();
}

void EventDispatcher::Select(int toolId)
{
    Unselect();
    selectedToolId = toolId;
    tools[selectedToolId]->OnSelect();
}

void EventDispatcher::Unselect()
{
    if (selectedToolId >= 0)
        tools[selectedToolId]->OnUnselect();
    selectedToolId = -1;
}

void EventDispatcher::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    if (selectedToolId >= 0)
        tools[selectedToolId]->OnCanvasLeftMouseButtonClick(event);
}

void EventDispatcher::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    if (selectedToolId >= 0)
        tools[selectedToolId]->OnCanvasMouseMotion(event);
}

void EventDispatcher::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
    if (selectedToolId >= 0)
        tools[selectedToolId]->OnCanvasRightMouseButtonRelease(event);
}