#include "eventdispatcher.hpp"

EventDispatcher::EventDispatcher(ToolSet tools, GLCanvas& canvas)
    : m_selectedToolId{-1}
    , m_canvas{canvas}
    , m_tools{std::move(tools)}
{
}

EventDispatcher::~EventDispatcher()
{
    Unselect();
}

void EventDispatcher::Select(int toolId)
{
    Unselect();
    m_selectedToolId = toolId;
    m_tools[m_selectedToolId]->OnSelect();
}

void EventDispatcher::Unselect()
{
    if (m_selectedToolId >= 0)
        m_tools[m_selectedToolId]->OnUnselect();
    m_selectedToolId = -1;
}

void EventDispatcher::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    m_canvas.HandleLeftMouseButtonClick(event);
    if (m_selectedToolId >= 0)
        m_tools[m_selectedToolId]->OnCanvasLeftMouseButtonClick(event);
}

void EventDispatcher::OnCanvasMouseMotion(const wxMouseEvent &event)
{
    m_canvas.HandleMouseMotion(event);
    if (m_selectedToolId >= 0)
        m_tools[m_selectedToolId]->OnCanvasMouseMotion(event);
}

void EventDispatcher::OnCanvasRightMouseButtonRelease(const wxMouseEvent &event)
{
    m_canvas.HandleRightMouseButtonRelease(event);
    if (m_selectedToolId >= 0)
        m_tools[m_selectedToolId]->OnCanvasRightMouseButtonRelease(event);
}