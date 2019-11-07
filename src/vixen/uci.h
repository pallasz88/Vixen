#pragma once

#include "defs.h"

namespace Vixen
{
    class Board;
}

namespace Vixen::Uci
{
    VIXEN_API void loop(Board &board);
}
