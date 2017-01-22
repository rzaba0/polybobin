#include "toolset.hpp"

ToolEventDispatcher::ToolEventDispatcher(ToolSet tools)
    : selectedToolId{-1}
    , tools{std::move(tools)}
{
}

ToolEventDispatcher::~ToolEventDispatcher()
{
    Unselect();
}

void ToolEventDispatcher::Select(int toolId)
{
    Unselect();
    selectedToolId = toolId;
    tools[selectedToolId]->OnSelect();
}

void ToolEventDispatcher::Unselect()
{
    if (selectedToolId >= 0)
    {
        tools[selectedToolId]->OnUnselect();
    }
    selectedToolId = -1;
}