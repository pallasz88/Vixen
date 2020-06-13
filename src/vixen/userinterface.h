#include "defs.h"

namespace Vixen
{
class MoveGenerator;

class Board;
} // namespace Vixen

namespace Vixen::UserInterface
{

/**
 * This function is responsible for interacting with user who should be able to:
 *      - print help
 *      - quit from program
 *      - reset board to start position
 *      - set  FEN position
 *      - print board
 *      - list available moves
 *      - make legal moves
 *      - undo moves
 * @param board
 */
VIXEN_API void WaitUserInput(Board &board);

/**
 * Makes the move if legal and prints new board.
 * @param move
 * @param board
 */
VIXEN_API void MakeMove(std::string &&move, Board &board);

/**
 * Undo last move
 * @param board
 */
VIXEN_API void TakeBackMove(Board &board);

/**
 * Prints all legal moves from current position.
 * @param board
 */
VIXEN_API void PrintMoveList(Board &board);

/**
 * Lists all available commands for user.
 */
VIXEN_API void PrintHelp();
} // namespace Vixen::UserInterface
