#define BOOST_TEST_MODULE TranspositionTableTests
#include <boost/test/included/unit_test.hpp>

#include "transposition_table.hpp"

namespace
{
constexpr vixen::Move move{1, 2, static_cast<std::uint8_t>(vixen::MoveTypes::QUIET_MOVE)};
}

BOOST_AUTO_TEST_CASE(empty_table_is_ignored)
{
    vixen::TranspositionTable table{0};

    table.Store(1, move, 10, 3, vixen::Bound::EXACT);
    BOOST_CHECK(!table.Probe(1).has_value());
}

BOOST_AUTO_TEST_CASE(uninitialized_entry_is_ignored)
{
    vixen::TranspositionTable table{1};
    BOOST_CHECK(!table.Probe(0).has_value());
}

BOOST_AUTO_TEST_CASE(entries_are_stored_and_cleared)
{
    vixen::TranspositionTable table{4};
    table.Store(1, move, 10, 3, vixen::Bound::EXACT);

    const auto entry = table.Probe(1);
    const bool stored = entry.has_value() && entry->key == 1 && entry->move == move && entry->score == 10 &&
                        entry->depth == 3 && entry->bound == vixen::Bound::EXACT;
    table.Clear();

    BOOST_CHECK(stored && !table.Probe(1).has_value());
}

BOOST_AUTO_TEST_CASE(deeper_entries_are_preserved)
{
    vixen::TranspositionTable table{1};
    table.Store(1, move, 10, 5, vixen::Bound::LOWER);
    table.Store(1, move, 20, 4, vixen::Bound::UPPER);

    const auto entry = table.Probe(1);
    BOOST_CHECK(entry.has_value() && entry->score == 10 && entry->depth == 5 &&
                entry->bound == vixen::Bound::LOWER);
}

BOOST_AUTO_TEST_CASE(collisions_replace_different_keys)
{
    vixen::TranspositionTable table{1};
    table.Store(1, move, 10, 5, vixen::Bound::LOWER);
    table.Store(2, move, 20, 1, vixen::Bound::UPPER);

    const auto entry = table.Probe(2);
    BOOST_CHECK(!table.Probe(1).has_value() && entry.has_value() && entry->score == 20 &&
                entry->bound == vixen::Bound::UPPER);
}
