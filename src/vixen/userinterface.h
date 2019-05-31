#include <memory>
#include <string>
#include "defs.h"

namespace Vixen
{
    class MoveGenerator;

    class Board;

    /**
     * UserInterface: handles user interaction
     */
    class VIXEN_API UserInterface
    {
    public:

        /*!
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
        static void WaitUserInput(Board &board);

        /**
         * Makes the move if legal and prints new board.
         * @param move
         * @param board
         */
        static void MakeMove(std::string &&move, Board &board);

        /**
         * Undo last move
         * @param board
         */
        static void TakeBackMove(Board &board);

        /**
         * Prints all legal moves from current position.
         * @param board
         */
        static void PrintMoveList(Board &board);

        /**
         * Lists all available commands for user.
         */
        static void PrintHelp();
    };
}
