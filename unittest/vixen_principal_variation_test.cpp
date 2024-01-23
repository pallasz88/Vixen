#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PVTable_TEST
#include <boost/test/included/unit_test.hpp>

#include "move.hpp"
#include "principal_variation.hpp"

BOOST_AUTO_TEST_CASE(StorePVEntryTest)
{
    constexpr std::size_t cacheSize{3U};
    vixen::PrincipalVariation pv{cacheSize};

    constexpr std::uint8_t moveTpye{static_cast<std::uint8_t>(vixen::MoveTypes::QUIET_MOVE)};

    constexpr vixen::Move move1(0, 1, moveTpye);
    constexpr vixen::Move move2(2, 3, moveTpye);
    constexpr vixen::Move move3(4, 5, moveTpye);
    constexpr vixen::Move move4(6, 7, moveTpye);

    // Test storing an entry with a new position key
    vixen::PVEntry entry1{move1, 1};
    pv.StorePVEntry(entry1);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(1).positionKey, 1);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(1).moveEntry, move1);

    // Test storing an entry with an existing position key
    vixen::PVEntry entry2{move2, 1};
    pv.StorePVEntry(entry2);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(1).positionKey, 1);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(1).moveEntry, move2);

    // Test storing an entry when the capacity is reached
    vixen::PVEntry entry3{move3, 2};
    pv.StorePVEntry(entry3);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(1).positionKey, 1);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(1).moveEntry, move2);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(2).positionKey, 2);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(2).moveEntry, move3);

    vixen::PVEntry entry4{move4, 3};
    pv.StorePVEntry(entry4);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(1).positionKey, 1);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(1).moveEntry, move2);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(2).positionKey, 2);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(2).moveEntry, move3);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(3).positionKey, 3);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(3).moveEntry, move4);
}

BOOST_AUTO_TEST_CASE(GetPVEntry_test)
{
    vixen::PrincipalVariation pv(2);

    constexpr std::uint8_t moveTpye{static_cast<std::uint8_t>(vixen::MoveTypes::QUIET_MOVE)};

    constexpr vixen::Move move1(0, 1, moveTpye);
    constexpr vixen::Move move2(2, 3, moveTpye);
    constexpr vixen::Move move3(4, 5, moveTpye);

    const vixen::PVEntry entry1{move1, 1};
    const vixen::PVEntry entry2{move2, 2};
    const vixen::PVEntry entry3{move3, 3};

    pv.StorePVEntry(entry1);
    pv.StorePVEntry(entry2);
    pv.StorePVEntry(entry3);

    // Check that the entry with position key 1 is not in the table
    BOOST_CHECK(pv.GetPVEntry(1) == vixen::PVEntry{});

    // Test retrieving an entry that exists in the hash table
    BOOST_CHECK_EQUAL(pv.GetPVEntry(2).moveEntry, entry2.moveEntry);
    BOOST_CHECK_EQUAL(pv.GetPVEntry(3).moveEntry, entry3.moveEntry);
}
