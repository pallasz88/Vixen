#define BOOST_TEST_MODULE FixedListTests
#include <boost/test/included/unit_test.hpp>

#include "fixed_list.hpp"

BOOST_AUTO_TEST_CASE(test_empty_fixed_list)
{
    FixedList<int, 5> list;
    BOOST_CHECK(list.empty());
    BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_non_empty_fixed_list)
{
    FixedList<int, 5> list;
    list.emplace_back(1);
    list.emplace_back(2);
    BOOST_CHECK(!list.empty());
    BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_fixed_list_iterator)
{
    FixedList<int, 5> list;
    list.emplace_back(1);
    list.emplace_back(2);

    int expected = 1;
    for (const auto &element : list)
    {
        BOOST_CHECK_EQUAL(element, expected++);
    }
}

BOOST_AUTO_TEST_CASE(test_fixed_list_indexing)
{
    FixedList<int, 5> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);

    BOOST_CHECK_EQUAL(list[0], 1);
    BOOST_CHECK_EQUAL(list[1], 2);
    BOOST_CHECK_EQUAL(list[2], 3);
}

BOOST_AUTO_TEST_CASE(test_fixed_list_begin_end)
{
    FixedList<int, 5> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);

    int expected = 1;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        BOOST_CHECK_EQUAL(*it, expected++);
    }
}

BOOST_AUTO_TEST_CASE(test_fixed_list_const_begin_end)
{
    const FixedList<int, 5> list = {1, 2, 3};

    int expected = 1;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        BOOST_CHECK_EQUAL(*it, expected++);
    }
}
