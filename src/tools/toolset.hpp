#pragma once
#include <vector>
#include <memory>
#include "tool.hpp"

using ToolSet = std::vector<std::unique_ptr<Tool>>;

class ToolEventDispatcher
{
public:
    ToolEventDispatcher() = delete;
    ToolEventDispatcher(ToolSet tools);
    void Select(int toolId);
    void Unselect();
    ~ToolEventDispatcher();
    
private:
    ToolSet tools;
    int selectedToolId;
};