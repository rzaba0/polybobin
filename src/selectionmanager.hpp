#pragma once
#include "gl/glcanvas.hpp"
#include "miniframes\displayframe.hpp"
#include "selection.hpp"
#include <vector>
#include <memory>

class SelectionManager
{
public:
    void PunctualSelection(wxPoint p);
    void RectangularSelection(wxPoint a, wxPoint b);
    SelectionManager(GLCanvas& canvas, const DisplayFrame& displayFrame);

private:
    GLCanvas& m_canvas;
    const DisplayFrame& m_displayFrame;
};