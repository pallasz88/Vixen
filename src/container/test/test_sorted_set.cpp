#include "sorted_set.hpp"
#include <gtest/gtest.h>
#include <random>

namespace
{

TEST(SORTED_SET, create_empty_set)
{
    container::SortedSet<int> set;
}

TEST(SORTED_SET, create_set_with_few_elements)
{
    container::SortedSet<int> set{1, 2, 3, 4, 5};
    EXPECT_TRUE(std::ranges::is_sorted(set));
    EXPECT_EQ(set.size(), 5);
}

TEST(SORTED_SET, create_set_with_few_unsorted_elements)
{
    container::SortedSet<int> set{2, 1, 3, 4, 5};
    EXPECT_TRUE(std::ranges::is_sorted(set));
}

TEST(SORTED_SET, find_element)
{
    container::SortedSet<int> set{1, 2, 3, 4, 5};
    EXPECT_EQ(*set.find(5), 5);
    EXPECT_EQ(set.find(5), begin(set) + 4);
}

TEST(SORTED_SET, not_find_element)
{
    container::SortedSet<int> set{1, 2, 3, 4, 5};
    EXPECT_EQ(set.find(6), set.end());
}

TEST(SORTED_SET, insert_element_to_empty_set)
{
    container::SortedSet<int> set;
    for (int i = 0; i < 15; ++i)
    {
        set.insert(i);
        EXPECT_TRUE(std::ranges::is_sorted(set));
    }
    EXPECT_EQ(set.size(), 15);
}

TEST(SORTED_SET, insert_random_element_to_empty_set)
{
    container::SortedSet<int> set;
    std::mt19937 rng(std::random_device{}());
    for (int i = 0; i < 1000; ++i)
    {
        set.insert(std::uniform_int_distribution<std::mt19937::result_type>{1, 1000}(rng));
        EXPECT_TRUE(std::ranges::is_sorted(set));
    }
    EXPECT_EQ(set.size(), 1000);
}

} // namespace