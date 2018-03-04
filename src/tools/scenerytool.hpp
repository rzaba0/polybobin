#pragma once
#include "tool.hpp"
#include "../gl/glcanvas.hpp"
#include "../miniframes/paletteframe.hpp"
#include "../settings.hpp"

class SceneryTool : public Tool
{
public:
    SceneryTool(GLCanvas& canvas, const Settings& settings, const PaletteFrame& palette);
    void OnSelect() override;
    void OnUnselect() override;
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
    void OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasMouseMotion(const wxMouseEvent &event) override;
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasKeyPress(const wxKeyEvent &event) override;
    void OnTimerTick() override;
    ~SceneryTool() override = default;

private:
    GLCanvas& m_canvas;
    const Settings& m_settings;
    const PaletteFrame& m_palette;

    bool m_movingScenery;
    int m_sceneryId;

    PMSScenery m_newScenery;
};
