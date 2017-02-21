#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "canvas.hpp"

class CanvasMock : public Canvas
{
public:
    MOCK_METHOD2(AddPolygon, int(PMSPolygon, PMSVertex));
    MOCK_METHOD4(EditPolygonVertex, void(unsigned, PMSPolygonType, unsigned, PMSVertex));
    MOCK_METHOD0(UpdatePolygonSelectionForRedraw, void(void));
    MOCK_CONST_METHOD1(GetPolygon, const PMSPolygon&(unsigned));
    MOCK_CONST_METHOD0(GetPolygonCount, unsigned(void));

    MOCK_METHOD2(SetBackgroundColors, void(wxColor, wxColor));
    MOCK_METHOD1(SetPolygonsTexture, void(wxString));

    MOCK_METHOD1(HandleLeftMouseButtonClick, void(const wxMouseEvent&));
    MOCK_METHOD1(HandleMouseMotion, void(const wxMouseEvent&));
    MOCK_METHOD1(HandleRightMouseButtonRelease, void(const wxMouseEvent&));
    MOCK_METHOD0(Draw, void(void));
};