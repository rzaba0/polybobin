#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <eventdispatcher.hpp>
#include "mock/tool_mock.hpp"
#include "mock/canvas_mock.hpp"

using Tool1Mock = ToolMock;
using Tool2Mock = ToolMock;

struct EventDispatcherTest : public testing::Test
{
    CanvasMock canvas;
    std::unique_ptr<EventDispatcher> sut;
    Tool1Mock* tool1;
    Tool2Mock* tool2;
    EventDispatcherTest()
    {
        ToolSet toolset;
        tool1 = new Tool1Mock();
        tool2 = new Tool2Mock();
        toolset.emplace_back(std::unique_ptr<Tool>(tool1));
        toolset.emplace_back(std::unique_ptr<Tool>(tool2));
        sut = std::make_unique<EventDispatcher>(std::move(toolset), canvas);
    }

    const wxMouseEvent dummyEvent = {};
};

TEST_F(EventDispatcherTest, onSelectAndOnUnselectEventsAreGenerated)
{
    testing::Sequence sequence;
    EXPECT_CALL(*tool1, OnSelect()).InSequence(sequence);
    EXPECT_CALL(*tool1, OnUnselect()).InSequence(sequence);
    EXPECT_CALL(*tool2, OnSelect()).InSequence(sequence);
    EXPECT_CALL(*tool2, OnUnselect()).InSequence(sequence);
    sut->Select(0);
    sut->Select(1);
}

TEST_F(EventDispatcherTest, eventsAreRedirectedToProperTool)
{
    testing::Sequence sequence;
    EXPECT_CALL(*tool2, OnSelect()).InSequence(sequence);
    EXPECT_CALL(canvas, HandleLeftMouseButtonClick(testing::_)).InSequence(sequence);
    EXPECT_CALL(*tool2, OnCanvasLeftMouseButtonClick(testing::_)).InSequence(sequence);
    EXPECT_CALL(*tool2, OnUnselect()).InSequence(sequence);
    EXPECT_CALL(*tool1, OnSelect()).InSequence(sequence);
    EXPECT_CALL(canvas, HandleMouseMotion(testing::_)).InSequence(sequence);
    EXPECT_CALL(*tool1, OnCanvasMouseMotion(testing::_)).InSequence(sequence);
    EXPECT_CALL(canvas, HandleRightMouseButtonRelease(testing::_)).InSequence(sequence);
    EXPECT_CALL(*tool1, OnCanvasRightMouseButtonRelease(testing::_)).InSequence(sequence);
    EXPECT_CALL(*tool1, OnUnselect()).InSequence(sequence);
    sut->Select(1);
    sut->OnCanvasLeftMouseButtonClick(dummyEvent);
    sut->Select(0);
    sut->OnCanvasMouseMotion(dummyEvent);
    sut->OnCanvasRightMouseButtonRelease(dummyEvent);
}