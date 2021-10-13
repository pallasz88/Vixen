#include "board.hpp"
#include "uci.hpp"

vixen::Hash::Keys vixen::Hash::zobristKeys;

int main()
{
    vixen::Uci uci;
    uci.loop();
    return 0;
}
