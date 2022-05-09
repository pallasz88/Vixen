#include "sorted_map.hpp"
#include <gtest/gtest.h>

namespace
{

TEST(SORTED_MAP, create_empty_map)
{
    container::SortedMap<int, int> map;
}

TEST(SORTED_MAP, create_map_with_few_elements)
{
    container::SortedMap<int, int> map{{1, 3}, {2, 4}};
    EXPECT_EQ(map[1], 3);
}

} // namespace