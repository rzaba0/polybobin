#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include <PolygonSelection.hpp>
#include <Selection.hpp>
#include <selectionmanager.hpp>
#include "mock/canvas_mock.hpp"

using namespace ::testing;

ACTION_P(ReturnFuncOfArgResult, f) { return f(arg0); }
ACTION_P(ReturnFuncResult, f) { return f(); }

class MapSimulator
{
    std::vector<PMSPolygon> polygons;
public:
    MapSimulator(std::vector<PMSPolygon> polygons) : polygons{ std::move(polygons) } { }
    const PMSPolygon& GetPolygon(unsigned id) const { return polygons[id]; }
    unsigned GetPolygonCount() const { return polygons.size(); }
};

struct SelectionManagerTest : public testing::Test
{
    // map consisting of two polygons.
    MapSimulator mapSim{{ 
        {PMSVertex{-10, -10}, PMSVertex{0, 10}, PMSVertex{10, -10}},
        {PMSVertex{20, 0}, PMSVertex{25, 7}, PMSVertex{30, 0}}
    }};
    CanvasMock canvas;
    DisplaySettings dummyDisplaySettings;
    PolygonSelection* polygonSelection;
    Selection* scenerySelection;
    SelectionManager sut;

    SelectionManagerTest()
        : polygonSelection{new PolygonSelection}
        , scenerySelection{new Selection}
        , sut{canvas, dummyDisplaySettings, std::unique_ptr<PolygonSelection>(polygonSelection), std::unique_ptr<Selection>(scenerySelection)}
    {
        // Inject our "map" into Canvas' GetPolygon, GetPolygonCount methods.
        ON_CALL(canvas, GetPolygonCount()).WillByDefault(ReturnFuncResult([&](){ return mapSim.GetPolygonCount(); }));
        ON_CALL(canvas, GetPolygon(_)).WillByDefault(ReturnFuncOfArgResult([&](unsigned id){ return mapSim.GetPolygon(id); }));
    }
};

TEST_F(SelectionManagerTest, selectUnselectAll)
{
    EXPECT_CALL(canvas, GetPolygonCount());
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());
    sut.SelectAll();
    EXPECT_EQ(mapSim.GetPolygonCount(), polygonSelection->size());
    EXPECT_CALL(canvas, Draw());
    sut.UnselectAll();
    EXPECT_TRUE(polygonSelection->empty());
}

TEST_F(SelectionManagerTest, punctualSelectUnselectTwoPolygons)
{
    const wxRealPoint clickPoint1{ 0, 0 };
    const wxRealPoint clickPoint2{ 25, 1 };
    const wxRealPoint clickPoint3{ 99, 99 };
    const unsigned expectedSelectedPolyId1 = 0;
    const unsigned expectedSelectedPolyId2 = 1;

    // clicking at first poly
    EXPECT_CALL(canvas, GetPolygonCount());
    EXPECT_CALL(canvas, GetPolygon(_)).Times(2);
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());

    sut.PunctualSelect(clickPoint1, true);
    EXPECT_EQ(1, polygonSelection->size());
    // entire polygon selected
    EXPECT_TRUE(polygonSelection->contains(0, 0));
    EXPECT_TRUE(polygonSelection->contains(0, 1));
    EXPECT_TRUE(polygonSelection->contains(0, 2));

    // clicking at second poly with "add" key
    EXPECT_CALL(canvas, GetPolygonCount());
    EXPECT_CALL(canvas, GetPolygon(_)).Times(2);
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());
    sut.PunctualSelect(clickPoint2, true);
    EXPECT_TRUE(polygonSelection->contains(expectedSelectedPolyId1));
    EXPECT_TRUE(polygonSelection->contains(expectedSelectedPolyId2));

    // clicking at first poly with "remove" key
    EXPECT_CALL(canvas, GetPolygonCount());
    EXPECT_CALL(canvas, GetPolygon(_)).Times(2);
    EXPECT_CALL(canvas, Draw());
    sut.PunctualUnselect(clickPoint1);
    EXPECT_FALSE(polygonSelection->contains(expectedSelectedPolyId1));
    EXPECT_TRUE(polygonSelection->contains(expectedSelectedPolyId2));

    // clicking outside of the polys
    EXPECT_CALL(canvas, GetPolygonCount());
    EXPECT_CALL(canvas, GetPolygon(_)).Times(2);
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());
    sut.PunctualSelect(clickPoint3);
    EXPECT_EQ(0, polygonSelection->size());
}

TEST_F(SelectionManagerTest, rectangularSelectUnselectTwoPolygons)
{
    const wxRealPoint clickPoint1{ 19, 1 };
    const wxRealPoint releasePoint1{ 21, -1 };

    const wxRealPoint clickPoint2{ 29, 1 };
    const wxRealPoint releasePoint2{ 31, -1 };

    // selecting first vertex of the second poly
    EXPECT_CALL(canvas, GetPolygonCount());
    EXPECT_CALL(canvas, GetPolygon(_)).Times(2);
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());
    sut.RectangularSelect(clickPoint1, releasePoint1);
    EXPECT_FALSE(polygonSelection->contains(0));
    EXPECT_TRUE(polygonSelection->contains(1, 0));
    EXPECT_FALSE(polygonSelection->contains(1, 1));
    EXPECT_FALSE(polygonSelection->contains(1, 2));

    // selecting the thrid vertex of the second poly with "add"
    EXPECT_CALL(canvas, GetPolygonCount());
    EXPECT_CALL(canvas, GetPolygon(_)).Times(2);
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());
    sut.RectangularSelect(clickPoint2, releasePoint2, true);
    EXPECT_FALSE(polygonSelection->contains(0));
    EXPECT_TRUE(polygonSelection->contains(1, 0));
    EXPECT_FALSE(polygonSelection->contains(1, 1));
    EXPECT_TRUE(polygonSelection->contains(1, 2));

    // unselecting the thrid vertex of the second polygon with "remove"
    EXPECT_CALL(canvas, GetPolygonCount());
    EXPECT_CALL(canvas, GetPolygon(_)).Times(2);
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());
    sut.RectangularUnselect(clickPoint1, releasePoint1);
    EXPECT_FALSE(polygonSelection->contains(1, 0));
    EXPECT_FALSE(polygonSelection->contains(1, 1));
    EXPECT_TRUE(polygonSelection->contains(1, 2));
}

TEST_F(SelectionManagerTest, completePolygonSelection)
{
    const wxRealPoint clickPoint1{ -11, -11 };
    const wxRealPoint releasePoint1{ -9, -9 };

    // selecting first vertex of the first poly
    EXPECT_CALL(canvas, GetPolygonCount()).Times(1);
    EXPECT_CALL(canvas, GetPolygon(_)).Times(2);
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw()).Times(2);
    EXPECT_CALL(canvas, Draw()).Times(2);
    sut.RectangularSelect(clickPoint1, releasePoint1);
    EXPECT_FALSE(polygonSelection->contains(1));
    EXPECT_TRUE(polygonSelection->contains(0, 0));
    EXPECT_FALSE(polygonSelection->contains(0, 1));
    EXPECT_FALSE(polygonSelection->contains(0, 2));
    sut.CompletePolygonSelection();
    EXPECT_TRUE(polygonSelection->contains(0, 0));
    EXPECT_TRUE(polygonSelection->contains(0, 1));
    EXPECT_TRUE(polygonSelection->contains(0, 2));
}

TEST_F(SelectionManagerTest, moveSelectedVerticles)
{
    const wxRealPoint clickPoint1{ -11, -11 };
    const wxRealPoint releasePoint1{ -9, -9 };

    const wxRealPoint clickPoint2{ 29, 1 };
    const wxRealPoint releasePoint2{ 31, -1 };

    const float vx = -2;
    const float vy = 3;
    
    // selecting first vertex of the first poly
    // and the thrid vertex of the second poly with "add"
    EXPECT_CALL(canvas, GetPolygonCount()).Times(2);
    EXPECT_CALL(canvas, GetPolygon(_)).Times(6);
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw()).Times(2);
    EXPECT_CALL(canvas, Draw()).Times(3);
    sut.RectangularSelect(clickPoint1, releasePoint1);
    sut.RectangularSelect(clickPoint2, releasePoint2, true);

    EXPECT_CALL(canvas, EditPolygonVertex(0, _, 0, _));
    EXPECT_CALL(canvas, EditPolygonVertex(1, _, 2, _));
    sut.MoveSelection(vx, vy);
};