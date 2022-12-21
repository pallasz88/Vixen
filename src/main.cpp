#include "board.hpp"
#include "uci.hpp"
#include <cstring>

vixen::Hash::Keys vixen::Hash::zobristKeys;

int main(int argc, char *argv[])
{
    vixen::Uci uci;
    if (argc > 1 && !std::strcmp(argv[1], "--benchmark"))
    {
        uci.benchmark();
    }
    else
    {
        uci.loop();
    }
    return 0;
}
