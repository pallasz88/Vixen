#include "userinterface.h"
#include "move_generator.h"
#include "board.h"

namespace Vixen
{
    void UserInterface::TakeBackMove(Board &board)
    {
        board.TakeBack();
    }
}
