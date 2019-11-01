#include <thread>
#include <timer.h>
#include <engine.h>

#include "userinterface.h"
#include "board.h"

#if defined _WIN32
#include "exchndl.h"
#endif

int main()
{
#if defined _WIN32
    ExcHndlInit();
#endif
    Vixen::Board board;
    board.SetBoard("3q2k1/4P3/8/5K2/8/8/8/8 w - - 0 1");
    //std::thread t(&Vixen::UserInterface::WaitUserInput, std::ref(board));
    //t.join();
    int alpha = -10000;
    int beta = 10000;
    Timer<std::chrono::milliseconds> t("AlpaBeta");
    std::cout << Vixen::Search::NegaMax(5, -beta, -alpha, board) << "\n";
    return 0;
}
