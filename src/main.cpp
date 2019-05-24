#include <iostream>
#include <boost/thread.hpp>
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
    Vixen::Board board1, board2;
    boost::thread t(Vixen::MoveGenerator::PerftTest, 5, std::ref(board1));
    std::cout << Vixen::MoveGenerator::PerftTest(5, board2) << std::endl;
    board2.PrintBoard();
    t.join();
    board1.PrintBoard();
    return 0;
}
