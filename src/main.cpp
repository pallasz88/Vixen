#include <iostream>
#include <thread>

#include "userinterface.h"
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
    std::thread t(&Vixen::UserInterface::WaitUserInput, std::ref(board));
    t.join();
    return 0;
}
