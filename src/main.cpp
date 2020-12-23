#include "board.h"
#include "uci.h"

Vixen::Hash::Keys Vixen::Hash::zobristKeys;

int main()
{
    Vixen::Uci uci;
    uci.loop();
    return 0;
}
