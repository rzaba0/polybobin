#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include <PolygonSelection.hpp>
#include <Selection.hpp>
#include <selectionmanager.hpp>
#include "mock/canvas_mock.hpp"

using namespace ::testing;

struct SelectionManagerTest : public testing::Test
{
    class MapSimulator
    {
        std::vector<PMSPolygon> polygons;
    public:
        MapSimulator(std::vector<PMSPolygon> polygons) : polygons{std::move(polygons)} { }
        const PMSPolygon& GetPolygon(unsigned id) const { return polygons[id]; }
        unsigned GetPolygonCount() const { return polygons.size(); }
    };
    
    // map consisting of two polygons.
    MapSimulator mapSim{{ 
        {PMSVertex{-10, -10}, PMSVertex{0, 10}, PMSVertex{10, -10}},
        {PMSVertex{20, 0}, PMSVertex{25, 7}, PMSVertex{30, 0}}
    }};

    CanvasMock canvas;
    DisplaySettings dummyDisplaySettings;
    PolygonSelection* polygonSelection{new PolygonSelection};
    Selection* scenerySelection{new Selection};
    SelectionManager sut{canvas, dummyDisplaySettings, std::unique_ptr<PolygonSelection>(polygonSelection), std::unique_ptr<Selection>(scenerySelection)};
};

TEST_F(SelectionManagerTest, selectUnselectAll)
{
    EXPECT_CALL(canvas, GetPolygonCount()).WillOnce(Invoke([&]() { return mapSim.GetPolygonCount(); }));
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());
    sut.SelectAll();
    EXPECT_EQ(mapSim.GetPolygonCount(), polygonSelection->size());
    EXPECT_CALL(canvas, Draw());
    sut.UnselectAll();
    EXPECT_TRUE(polygonSelection->empty());
}

TEST_F(SelectionManagerTest, punctualSelect)
{
    const wxRealPoint clickPoint1{ 0, 5 };
    const wxRealPoint clickPoint2{ 25, 1 };
    const unsigned expectedSelectedPolyId1 = 0;
    const unsigned expectedSelectedPolyId2 = 1;

    // clicking at first poly
    EXPECT_CALL(canvas, GetPolygonCount()).WillOnce(Invoke([&]() { return mapSim.GetPolygonCount(); }));
    EXPECT_CALL(canvas, GetPolygon(expectedSelectedPolyId1)).WillOnce(Invoke([&](unsigned polyId) { return mapSim.GetPolygon(polyId); }));
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());

    sut.PunctualSelect(clickPoint1, true);
    EXPECT_EQ(1, polygonSelection->size());
    // entire polygon selected
    EXPECT_TRUE(polygonSelection->contains(0, 0));
    EXPECT_TRUE(polygonSelection->contains(0, 1));
    EXPECT_TRUE(polygonSelection->contains(0, 2));

    // clicking at second poly with "add" key
    EXPECT_CALL(canvas, GetPolygonCount()).WillOnce(Invoke([&]() { return mapSim.GetPolygonCount(); }));
    EXPECT_CALL(canvas, GetPolygon(expectedSelectedPolyId2)).WillOnce(Invoke([&]() { return mapSim.GetPolygon(expectedSelectedPolyId2); }));
    EXPECT_CALL(canvas, UpdatePolygonSelectionForRedraw());
    EXPECT_CALL(canvas, Draw());
    sut.PunctualSelect(clickPoint2);
    EXPECT_TRUE(polygonSelection->contains(expectedSelectedPolyId1));
    EXPECT_TRUE(polygonSelection->contains(expectedSelectedPolyId2));
}

TEST_F(SelectionManagerTest, last)
{
}