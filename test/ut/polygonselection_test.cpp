#include <gtest/gtest.h>
#include <polygonselection.hpp>

struct polygonSelectionTest : public testing::Test
{
    PolygonSelection sut;
};

TEST_F(polygonSelectionTest, selectPoly)
{
    sut.selectAll(3);
    EXPECT_TRUE(sut.contains(0));
    EXPECT_TRUE(sut.contains(0, 0));
    EXPECT_TRUE(sut.contains(0, 1));
    EXPECT_TRUE(sut.contains(0, 2));
    EXPECT_TRUE(sut.contains(1));
    EXPECT_TRUE(sut.contains(2));
    EXPECT_FALSE(sut.contains(3));
    sut.select(4);
    EXPECT_TRUE(sut.contains(4));
}

TEST_F(polygonSelectionTest, unselectPoly)
{
    sut.selectAll(3);
    sut.unselect(1);
    EXPECT_TRUE(sut.contains(0));
    EXPECT_FALSE(sut.contains(1));
    EXPECT_TRUE(sut.contains(2));
    sut.unselectAll();
    EXPECT_TRUE(sut.empty());
}

TEST_F(polygonSelectionTest, selectSingleVertex)
{
    sut.select(10, 1);
    EXPECT_FALSE(sut.contains(10, 0));
    EXPECT_TRUE(sut.contains(10, 1));
    EXPECT_FALSE(sut.contains(10, 2));
    sut.select(10, 2);
    EXPECT_TRUE(sut.contains(10, 2));
    EXPECT_EQ(1, sut.size());
}

TEST_F(polygonSelectionTest, unselectSingleVertex)
{
    sut.select(0);
    sut.unselect(0, 1);
    EXPECT_TRUE(sut.contains(0, 0));
    EXPECT_FALSE(sut.contains(0, 1));
    EXPECT_TRUE(sut.contains(0, 2));
    sut.unselect(0, 0);
    EXPECT_FALSE(sut.contains(0, 0));
    sut.unselect(0, 2);
    EXPECT_TRUE(sut.empty());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
