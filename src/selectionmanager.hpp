#pragma once

#include "canvas.hpp"
#include "gl/displaysettings.hpp"
#include "selection.hpp"
#include "polygonselection.hpp"
#include <vector>
#include <functional>
#include <memory>

class SelectionManager
{
public:
    void SelectAll();
    void UnselectAll();

    void PunctualSelect(wxRealPoint p, bool append = false);
    void RectangularSelect(wxRealPoint a, wxRealPoint b, bool append = false);

    void PunctualUnselect(wxRealPoint p);
    void RectangularUnselect(wxRealPoint a, wxRealPoint b);

    void CompletePolygonSelection();

    void MoveSelection(float vx, float vy);

    void RemoveSelection();

    SelectionManager(Canvas& canvas, const DisplaySettings& displaySettings, std::unique_ptr<PolygonSelection> polygonSelection, std::unique_ptr<Selection> scenerySelection);

private:
    void ForEachPolyAtPosition(wxRealPoint position, std::function<void(unsigned)> operation);
    void ForEachVerticleInRectangle(wxRealPoint a, wxRealPoint b, std::function<void(unsigned, unsigned)> operation);

    Canvas& m_canvas;
    const DisplaySettings& m_displaySettings;
    std::unique_ptr<PolygonSelection> m_polygonSelection;
    std::unique_ptr<Selection> m_scenerySelection;
};