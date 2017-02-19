#include "eventdispatcherfactory.hpp"
#include "tools/transformtool.hpp"
#include "tools/polygontool.hpp"
#include "tools/vertexselectiontool.hpp"
#include "tools/selectiontool.hpp"
#include "mainframe.hpp"

std::unique_ptr<EventDispatcher> MakeEventDispatcher(GLCanvas& canvas, MainFrame& mainFrame)
{
    ToolSet toolSet;
    toolSet.emplace_back(std::make_unique<TransformTool>());
    toolSet.emplace_back(std::make_unique<PolygonTool>(canvas, mainFrame.GetPaletteFrame()));
    toolSet.emplace_back(std::make_unique<VertexSelectionTool>());
    toolSet.emplace_back(std::make_unique<SelectionTool>());
    std::unique_ptr<EventDispatcher> dispatcher = std::make_unique<EventDispatcher>(std::move(toolSet), canvas);

    canvas.Bind(wxEVT_LEFT_DOWN, &EventDispatcher::OnCanvasLeftMouseButtonClick, dispatcher.get());
    canvas.Bind(wxEVT_RIGHT_UP,  &EventDispatcher::OnCanvasRightMouseButtonRelease, dispatcher.get());
    canvas.Bind(wxEVT_MOTION,    &EventDispatcher::OnCanvasMouseMotion, dispatcher.get());

    return dispatcher;
}