#include <gtest/gtest.h>
#include <bt_map.h>
#include <iostream>
#include <sstream>


using namespace bt;

const char* mapData = ".........................."
                    ".........................."
                    "..##..##..##..##..##..##.."
                    "..##..##..##..##..##..##.."
                    "..##..##..##..##..##..##.."
                    "..##..##..##..##..##..##.."
                    "..##..##..##@@##..##..##.."
                    "..##..##..##@@##..##..##.."
                    "..##..##..##..##..##..##.."
                    "..##..##..........##..##.."
                    "..##..##..........##..##.."
                    "..........##..##.........."
                    "..........##..##.........."
                    "##..####..........####..##"
                    "@@..####..........####..@@"
                    "..........##..##.........."
                    "..........######.........."
                    "..##..##..######..##..##.."
                    "..##..##..##..##..##..##.."
                    "..##..##..##..##..##..##.."
                    "..##..##..##..##..##..##.."
                    "..##..##..........##..##.."
                    "..##..##..........##..##.."
                    "..##..##...####...##..##.."
                    "...........#..#..........."
                    "...........#..#...........";

TEST(MAP, load_from_stream) {
    std::stringstream ss;
    ss << mapData;

    Map map;
    map.load(ss);

    const char* c = mapData;

    for (size_t x = 0; x != map.GRID_SIZE; ++x) {
        for (size_t y = 0; y != map.GRID_SIZE; ++y) {
            EXPECT_EQ(*c, map.state(x, y));
            ++c;
        }
    }
}


TEST(MAP, is_free_true) {
    std::stringstream ss;
    ss << mapData;

    Map map;
    map.load(ss);

    // first cell is free
    EXPECT_TRUE(map.isFree(0, 0, map.CELL_SIZE, map.CELL_SIZE));
    // second row is free
    EXPECT_TRUE(map.isFree(0, 0, map.CELL_SIZE * map.GRID_SIZE, map.CELL_SIZE));

    // first two left columns are free
    EXPECT_TRUE(map.isFree(0, 4 * map.CELL_SIZE, map.CELL_SIZE * 2, map.CELL_SIZE));

    EXPECT_TRUE(map.isFree(0, 2 * map.CELL_SIZE, 2 * map.CELL_SIZE, map.CELL_SIZE));

}


TEST(MAP, is_free_false) {
    std::stringstream ss;
    ss << mapData;

    Map map;
    map.load(ss);

    EXPECT_FALSE(map.isFree(0, 2 * map.CELL_SIZE, 2 * map.CELL_SIZE + 7, map.CELL_SIZE));
}
