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

        void MakeMove(std::string move, Board &board);

        void TakeBackMove(Board &board);
    };
}
