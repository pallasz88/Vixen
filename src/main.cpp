#include <iostream>
#include "board.h"
#include "hash.h"
#include "move_generator.h"
#if defined _WIN32
#include "exchndl.h"
#endif

int main()
{
#if defined _WIN32
    ExcHndlInit();
#endif
    Vixen::Board board;
    //std::cout << "Size of board in bytes: " << sizeof(board) << std::endl;
    //board.SetBoard("8/8/8/8/3P4/8/8/8 w - - 0 1");
    //board.MakeMove(2332);
    std::cout << Vixen::MoveGenerator::PerftTest(5, board) << std::endl;
    board.PrintBoard();
    return 0;
}
