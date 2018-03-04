#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "canvas.hpp"

class CanvasMock : public Canvas
{
public:
    MOCK_METHOD2(AddPolygon, int(PMSPolygon, PMSVertex));
    MOCK_METHOD1(AddScenery, int(PMSScenery));
    MOCK_METHOD1(AddSceneryType, int(wxString));
    MOCK_METHOD1(AddSpawnPoint, int(PMSSpawnPoint));

    MOCK_METHOD4(EditPolygonVertex, void(unsigned, PMSPolygonType, unsigned, PMSVertex));
    MOCK_METHOD2(EditScenery, void(unsigned, PMSScenery));
    MOCK_METHOD2(EditSpawnPoint, void(unsigned int, PMSSpawnPoint));

    MOCK_METHOD1(RemovePolygons, void(const wxVector<unsigned int>&));
    MOCK_METHOD1(RemoveSpawnPoints, void(wxVector<unsigned int>));
    MOCK_METHOD1(RemoveSceneries, void(const wxVector<unsigned int>&));
    MOCK_METHOD1(RemoveScenery, void(unsigned int));

    MOCK_METHOD0(UpdatePolygonSelectionForRedraw, void(void));
    MOCK_CONST_METHOD1(GetPolygon, const PMSPolygon&(unsigned));
    MOCK_CONST_METHOD0(GetPolygonCount, unsigned(void));
    MOCK_CONST_METHOD1(GetMousePositionOnMap, wxRealPoint(wxPoint));

    MOCK_METHOD2(SetBackgroundColors, void(wxColor, wxColor));
    MOCK_METHOD1(SetPolygonsTexture, void(wxString));

    MOCK_METHOD1(HandleLeftMouseButtonClick, void(const wxMouseEvent&));
    MOCK_METHOD1(HandleMouseMotion, void(const wxMouseEvent&));
    MOCK_METHOD1(HandleRightMouseButtonRelease, void(const wxMouseEvent&));
    MOCK_METHOD1(HandleKeyPress, void(const wxKeyEvent&));
    MOCK_METHOD0(Draw, void(void));
};
