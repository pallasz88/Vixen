#include <iostream>
#include "board.h"
#include "hash.h"
#include "move_generator.h"
#include "exchndl.h"

int main()
{
    ExcHndlInit();
    Vixen::Board board;
    board.SetBoard("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1");
    board.PrintBoard();
    Timer t("MAIN");
    std::cout << Vixen::MoveGenerator::PerftTest(4, board) << std::endl;
    //board.MakeMove(5835);
    //board.PrintBoard();
    //board.TakeBack();
    //board.PrintBoard();
    return 0;
}
