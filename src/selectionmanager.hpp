#pragma once

#include "canvas.hpp"
#include "gl/displaysettings.hpp"
#include "selection.hpp"
#include "polygonselection.hpp"
#include <vector>
#include <memory>

class SelectionManager
{
public:
    void SelectAll();
    void UnselectAll();
   
    void PunctualSelect(wxPoint p, bool append = false);
    void RectangularSelect(wxPoint a, wxPoint b, bool append = false);
    
    void PunctualUnselect(wxPoint p);
    void RectangularUnselect(wxPoint a, wxPoint b);
    
    void CompletePolygonSelection();

    void MoveSelection(float vx, float vy);

    SelectionManager(Canvas& canvas, const DisplaySettings& displayFrame, std::unique_ptr<PolygonSelection> polygonSelection, std::unique_ptr<Selection> scenerySelection);

private:
    Canvas& m_canvas;
    const DisplaySettings& m_displaySettings;
    std::unique_ptr<PolygonSelection> m_polygonSelection;
    std::unique_ptr<Selection> m_scenerySelection;
};