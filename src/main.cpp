#include "board.hpp"
#include "uci.hpp"

Vixen::Hash::Keys Vixen::Hash::zobristKeys;

int main()
{
    Vixen::Uci uci;
    uci.loop();
    return 0;
}
