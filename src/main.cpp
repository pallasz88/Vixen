#include <thread>

#include "userinterface.h"
#include "board.h"
#include "uci.h"

int main()
{
    Vixen::Uci uci;
//    board.SetBoard("3q2k1/4P3/8/5K2/8/8/8/8 w - - 0 1");
//    Vixen::UserInterface::WaitUserInput(board);
//    int alpha = -10000;
//    int beta = 10000;
//    Timer<std::chrono::milliseconds> t("AlpaBeta");
//    std::cout << Vixen::Search::NegaMax(1, -beta, -alpha, board) << "\n";
    uci.loop();
    return 0;
}
