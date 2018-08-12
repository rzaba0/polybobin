#pragma once

#include "tool.hpp"

#include "../selectionmanager.hpp"
#include "../miniframes/paletteframe.hpp"
#include "../gl/glcanvas.hpp"

class ColorTool : public Tool
{
public:
    ColorTool(GLCanvas& canvas,
            SelectionManager& selectionManager,
            const PaletteFrame& palette);
    void OnSelect() override;
    void OnUnselect() override;
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
    void OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasMouseMotion(const wxMouseEvent &event) override;
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasKeyPress(const wxKeyEvent &event) override;
    void OnTimerTick() override;
    ~ColorTool() override = default;

private:
    GLCanvas& m_canvas;
    SelectionManager& m_selectionManager;
    const PaletteFrame& m_palette;
};
