// Kell egy interface amin keresztül lehet lépést:
// - bevinni 
// - visszavenni
//
//
#include <memory>
#include <string>
#include "defs.h"

namespace Vixen
{
    class MoveGenerator;

    class Board;

    class VIXEN_API UserInterface
    {
    public:

        static void WaitUserInput(Board &board);

        static void MakeMove(std::string &&move, Board &board);

        static void TakeBackMove(Board &board);
    };
}
