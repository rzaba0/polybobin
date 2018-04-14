#pragma once
#include "tool.hpp"
#include "../gl/glcanvas.hpp"
#include "../miniframes/paletteframe.hpp"
#include "../miniframes/sceneryframe.hpp"
#include "../settings.hpp"

class SceneryTool : public Tool
{
public:
    SceneryTool(GLCanvas& canvas, const Settings& settings, SceneryFrame& sceneryFrame, const PaletteFrame& palette);
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
    void OnSceneryFrameSelect(wxListEvent& event);

    GLCanvas& m_canvas;
    const Settings& m_settings;
    SceneryFrame& m_sceneryFrame;
    const PaletteFrame& m_palette;

    bool m_toolSelected;

    wxString m_currentSceneryTypeName;
    bool m_editingScenery;
    bool m_movingScenery;
    bool m_rotatingScenery;
    bool m_scalingScenery;
    int m_sceneryId;

    PMSScenery m_newScenery;
};
