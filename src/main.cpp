#include <iostream>
#include "board.h"
#include "hash.h"
#include "move_generator.h"
#include "exchndl.h"

int main()
{
    ExcHndlInit();
    Vixen::Board board;
    board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    board.PrintBoard();
    Timer t("MAIN");
    std::cout << Vixen::MoveGenerator::PerftTest(2, board) << std::endl;
    //board.MakeMove(5835);
    //board.PrintBoard();
    //board.TakeBack();
    //board.PrintBoard();
    return 0;
}
