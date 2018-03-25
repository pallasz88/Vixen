#include <UnitTest++.h>
#include "Board.h"
#include "defs.h"

TEST(Test_Startposition)
{
    CHECK_EQUAL(                  8, CURRENT_BOARD()->GetBitBoards().K);
    CHECK_EQUAL(                 16, CURRENT_BOARD()->GetBitBoards().Q);
    CHECK_EQUAL(                129, CURRENT_BOARD()->GetBitBoards().R);
    CHECK_EQUAL(                 36, CURRENT_BOARD()->GetBitBoards().B);
    CHECK_EQUAL(                 66, CURRENT_BOARD()->GetBitBoards().N);
    CHECK_EQUAL(              RANK2, CURRENT_BOARD()->GetBitBoards().P);

    CHECK_EQUAL(              RANK7, CURRENT_BOARD()->GetBitBoards().p);
    CHECK_EQUAL(4755801206503243776, CURRENT_BOARD()->GetBitBoards().n);
    CHECK_EQUAL(2594073385365405696, CURRENT_BOARD()->GetBitBoards().b);
    //CHECK_EQUAL(9295429630892703744, CURRENT_BOARD()->GetBitBoards().r);
    CHECK_EQUAL(1152921504606846976, CURRENT_BOARD()->GetBitBoards().q);
    CHECK_EQUAL( 576460752303423488, CURRENT_BOARD()->GetBitBoards().k);

    CHECK_EQUAL(true, CURRENT_BOARD()->IsWhiteToMove());
}

TEST(Test_SetPosition)
{
    CURRENT_BOARD()->SetBoard(TESTPOS1);
    CURRENT_BOARD()->PrintBoard();

    CHECK_EQUAL(           2, CURRENT_BOARD()->GetBitBoards().K);
    CHECK_EQUAL(      524288, CURRENT_BOARD()->GetBitBoards().Q);
    CHECK_EQUAL(         132, CURRENT_BOARD()->GetBitBoards().R);
    CHECK_EQUAL(    67109376, CURRENT_BOARD()->GetBitBoards().B);
    CHECK_EQUAL(        4096, CURRENT_BOARD()->GetBitBoards().N);
    CHECK_EQUAL(103081542656, CURRENT_BOARD()->GetBitBoards().P);

    CHECK_EQUAL(   3802523525709824, CURRENT_BOARD()->GetBitBoards().p);
    CHECK_EQUAL(          536870912, CURRENT_BOARD()->GetBitBoards().n);
    CHECK_EQUAL(   5066549580791808, CURRENT_BOARD()->GetBitBoards().b);
    CHECK_EQUAL(2594073385365405696, CURRENT_BOARD()->GetBitBoards().r);
    CHECK_EQUAL(1152921504606846976, CURRENT_BOARD()->GetBitBoards().q);
    CHECK_EQUAL( 144115188075855872, CURRENT_BOARD()->GetBitBoards().k);

    CHECK_EQUAL(true, CURRENT_BOARD()->IsWhiteToMove());
}

int main()
{
    return UnitTest::RunAllTests();
}
