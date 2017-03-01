#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "tools/tool.hpp"

class ToolMock : public Tool
{
public:
    MOCK_METHOD0(OnSelect, void());
    MOCK_METHOD0(OnUnselect, void());
    MOCK_METHOD1(OnCanvasLeftMouseButtonClick, void(const wxMouseEvent&));
    MOCK_METHOD1(OnCanvasLeftMouseButtonRelease, void(const wxMouseEvent&));
    MOCK_METHOD1(OnCanvasMouseMotion, void(const wxMouseEvent&));
    MOCK_METHOD1(OnCanvasRightMouseButtonRelease, void(const wxMouseEvent&));
    MOCK_METHOD0(OnTimerTick, void());
};