#include "eventdispatcherfactory.hpp"
#include "tools/transformtool.hpp"
#include "tools/polygontool.hpp"
#include "tools/vertexselectiontool.hpp"
#include "tools/selectiontool.hpp"
#include "tools/scenerytool.hpp"
#include "tools/spawnpointtool.hpp"
#include "tools/colortool.hpp"
#include "tools/vertexcolortool.hpp"
#include "tools/texturetool.hpp"
#include "tools/waypointtool.hpp"
#include "tools/colorpickertool.hpp"
#include "tools/sketchtool.hpp"
#include "tools/lightstool.hpp"
#include "tools/depthtool.hpp"
#include "mainframe.hpp"

std::unique_ptr<EventDispatcher> CreateEventDispatcher(GLCanvas& canvas, MainFrame& mainFrame, SelectionManager& selectionManager)
{
    ToolSet toolSet;
    toolSet.emplace_back(std::make_unique<TransformTool>(selectionManager, canvas));
    toolSet.emplace_back(std::make_unique<PolygonTool>(canvas, mainFrame.GetPaletteFrame()));
    toolSet.emplace_back(std::make_unique<VertexSelectionTool>(selectionManager, canvas));
    toolSet.emplace_back(std::make_unique<SelectionTool>(selectionManager, canvas));
    toolSet.emplace_back(std::make_unique<VertexColorTool>());
    toolSet.emplace_back(std::make_unique<ColorTool>(canvas, selectionManager, mainFrame.GetPaletteFrame()));
    toolSet.emplace_back(std::make_unique<TextureTool>(selectionManager, canvas));
    toolSet.emplace_back(std::make_unique<SceneryTool>(canvas, mainFrame.GetSettings(),
            mainFrame.GetSceneryFrame(), mainFrame.GetPaletteFrame()));
    toolSet.emplace_back(std::make_unique<WaypointTool>());
    toolSet.emplace_back(std::make_unique<SpawnPointTool>(canvas));
    toolSet.emplace_back(std::make_unique<ColorPickerTool>(canvas, selectionManager, mainFrame.GetPaletteFrame()));
    toolSet.emplace_back(std::make_unique<SketchTool>()); // TODO: useless tool, it will be deleted
    toolSet.emplace_back(std::make_unique<LightsTool>()); // TODO: useless tool, it will be deleted
    toolSet.emplace_back(std::make_unique<DepthTool>()); // TODO: useless tool, it will be deleted
    std::unique_ptr<EventDispatcher> dispatcher = std::make_unique<EventDispatcher>(std::move(toolSet), canvas);

    canvas.Bind(wxEVT_LEFT_DOWN, &EventDispatcher::OnCanvasLeftMouseButtonClick, dispatcher.get());
    canvas.Bind(wxEVT_LEFT_UP,   &EventDispatcher::OnCanvasLeftMouseButtonRelease, dispatcher.get());
    canvas.Bind(wxEVT_RIGHT_UP,  &EventDispatcher::OnCanvasRightMouseButtonRelease, dispatcher.get());
    canvas.Bind(wxEVT_MOTION,    &EventDispatcher::OnCanvasMouseMotion, dispatcher.get());
    canvas.Bind(wxEVT_CHAR_HOOK, &EventDispatcher::OnCanvasKeyPress, dispatcher.get());

    return dispatcher;
}
