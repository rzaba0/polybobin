#pragma once

#include "tool.hpp"

#include "../selectionmanager.hpp"
#include "../miniframes/paletteframe.hpp"
#include "../gl/glcanvas.hpp"

class ColorPickerTool : public Tool
{
public:
    ColorPickerTool(GLCanvas& canvas,
            SelectionManager& selectionManager,
            PaletteFrame& palette);
    void OnSelect() override;
    void OnUnselect() override;
    void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
    void OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasMouseMotion(const wxMouseEvent &event) override;
    void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
    void OnCanvasKeyPress(const wxKeyEvent &event) override;
    void OnTimerTick() override;
    ~ColorPickerTool() override = default;

private:
    GLCanvas& m_canvas;
    SelectionManager& m_selectionManager;
    PaletteFrame& m_palette;
};
