#include <gtest/gtest.h>
#include <bt_model.h>

using namespace bt;

TEST(MODEL, get_next_position_up) {

    Model model(0, 500, 26, 26, 0.08, Model::UP);

    auto position = model.getNextPosition(50);

    EXPECT_EQ(0, position.first);
    EXPECT_EQ(500 - 0.08 * 50, position.second);
}


TEST(MODEL, get_next_position_down) {

    Model model(0, 500, 26, 26, 0.08, Model::DOWN);

    auto position = model.getNextPosition(50);

    EXPECT_EQ(0, position.first);
    EXPECT_EQ(500 + 0.08 * 50, position.second);
}


TEST(MODEL, get_next_position_left) {

    Model model(100, 500, 26, 26, 0.08, Model::LEFT);

    auto position = model.getNextPosition(50);

    EXPECT_EQ(100 - 0.08 * 50, position.first);
    EXPECT_EQ(500, position.second);
}


TEST(MODEL, get_next_position_right) {

    Model model(100, 500, 26, 26, 0.08, Model::RIGHT);

    auto position = model.getNextPosition(50);

    EXPECT_EQ(100 + 0.08 * 50, position.first);
    EXPECT_EQ(500, position.second);
}