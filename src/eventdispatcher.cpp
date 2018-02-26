#include "eventdispatcher.hpp"
#include "constants.hpp"

EventDispatcher::EventDispatcher(ToolSet tools, Canvas& canvas)
    : m_tools{std::move(tools)}
    , m_canvas{canvas}
    , m_selectedToolId{-1}
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

int EventDispatcher::Unselect()
{
    int lastSelectedToolId = m_selectedToolId;
    if (m_selectedToolId >= 0)
        m_tools[m_selectedToolId]->OnUnselect();
    m_selectedToolId = -1;

    return lastSelectedToolId;
}

void EventDispatcher::OnCanvasLeftMouseButtonClick(const wxMouseEvent &event)
{
    m_canvas.HandleLeftMouseButtonClick(event);
    if (m_selectedToolId >= 0)
        m_tools[m_selectedToolId]->OnCanvasLeftMouseButtonClick(event);
}

void EventDispatcher::OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event)
{
    if (m_selectedToolId >= 0)
        m_tools[m_selectedToolId]->OnCanvasLeftMouseButtonRelease(event);
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

void EventDispatcher::OnCanvasKeyPress(wxKeyEvent &event)
{
    m_canvas.HandleKeyPress(event);
	if (event.GetKeyCode() == REMOVE_SELECTED_KEY && m_selectedToolId != ID_TOOL_SELECTION - ID_TOOL_TRANSFORM)
		m_tools[ID_TOOL_SELECTION - ID_TOOL_TRANSFORM]->OnCanvasKeyPress(event);
    if (m_selectedToolId >= 0)
        m_tools[m_selectedToolId]->OnCanvasKeyPress(event);
    event.Skip();
}
