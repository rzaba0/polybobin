#include <gtest/gtest.h>
#include <selection.hpp>

struct selectionTest : public testing::Test
{
    Selection sut;
};

TEST_F(selectionTest, select)
{
    sut.selectAll(3);
    EXPECT_TRUE(sut.contains(0));
    EXPECT_TRUE(sut.contains(1));
    EXPECT_TRUE(sut.contains(2));
    EXPECT_FALSE(sut.contains(3));
    sut.select(4);
    EXPECT_TRUE(sut.contains(4));
}

TEST_F(selectionTest, unselect)
{
    sut.selectAll(3);
    sut.unselect(1);
    EXPECT_TRUE(sut.contains(0));
    EXPECT_FALSE(sut.contains(1));
    EXPECT_TRUE(sut.contains(2));
    sut.unselectAll();
    EXPECT_TRUE(sut.empty());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
