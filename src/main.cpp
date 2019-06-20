#include <iostream>
#include <thread>

#include "userinterface.h"
#include "board.h"
#include "hash.h"
#include "move_generator.h"
//#include "engine.h"
//#include "timer.h"

#if defined _WIN32
#include "exchndl.h"
#endif

int main()
{
#if defined _WIN32
    ExcHndlInit();
#endif
    Vixen::Board board;
    std::thread t(&Vixen::UserInterface::WaitUserInput, std::ref(board));
    t.join();
//    Timer<std::chrono::milliseconds> t("AlpaBeta");
//    std::cout << Vixen::Search::AlphaBeta(5,-1000,1000, board) << "\n";
    return 0;
}
