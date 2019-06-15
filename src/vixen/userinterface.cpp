#include <iostream>
#include <regex>
#include "userinterface.h"
#include "move_generator.h"
#include "board.h"

namespace Vixen::UserInterface
{
    void WaitUserInput(Vixen::Board &board)
    {
        while (true)
        {
            std::string command;
            std::cout << "Vixen >";
            std::getline(std::cin, command);

            if (command == "quit")
                return;

            else if (command == "help")
                PrintHelp();

            else if (command.substr(0, 8) == "position")
                board.SetBoard(command.substr(10));

            else if (command == "print")
                board.PrintBoard();

            else if (command == "reset")
                board.SetBoard(START_POSITION);

            else if (command.substr(0, 4) == "move")
                try
                {
                    UserInterface::MakeMove(command.substr(5), board);
                } catch (std::runtime_error &error)
                {
                    std::cerr << error.what() << "\n";
                } catch (std::out_of_range &error)
                {
                    std::cerr << "INVALID MOVE FORMAT. Use for example e2e4.\n";
                }

            else if (command == "undo")
                try
                {
                    TakeBackMove(board);
                } catch (std::runtime_error &error)
                {
                    std::cerr << "ERROR HAPPENED: " << error.what() << "\n";
                }

            else if (command == "list")
                PrintMoveList(board);

            else
                std::cerr << "UNKNOWN COMMAND: " << command << "\n";
        }
    }

    void MakeMove(std::string &&move, Vixen::Board &board)
    {
        bool isLegal = false;
        std::cout << "Your move is: " << move << "\n";
        std::regex moveRegex{"[a-h][1-8][a-h][1-8]"};
        if (!std::regex_match(move, moveRegex))
            throw std::runtime_error("INVALID MOVE FORMAT. Use for example e2e4.");

        int from = NotationToSquare(move.substr(0, 2));
        int to = NotationToSquare(move.substr(2));
        int decodedMove = to << 6 | from;
        std::vector<Move> moves = board.GetAllMoves();

        for (const auto &pseudoMove : moves)
            if ((pseudoMove & 0xFFF) == decodedMove)
            {
                if (!board.MakeMove(pseudoMove))
                    throw std::runtime_error("Illegal move!");
                isLegal = true;
                break;
            }

        if (!isLegal)
            throw std::runtime_error("Illegal move!");

        board.PrintBoard();

    }

    void TakeBackMove(Board &board)
    {
        board.TakeBack();
    }

    void PrintMoveList(Board &board)
    {
        for (const auto &move : board.GetAllMoves())
        {
            if (board.MakeMove(move))
            {
                std::cout << SquareToNotation(move & 0x3F)
                          << SquareToNotation((move & 0xFC0) >> 6);
                if ((move >> 12) & PROMOTION)
                    std::cout << "nbrq"[static_cast<int>((move >> 12) & 3)];

                std::cout << ", ";
                board.TakeBack();
            }
        }
        std::cout << "\n";
    }

    void PrintHelp()
    {
        std::cout << "Welcome to Vixen C++ chess engine!\n";
        std::cout << "\nUse the below listed commands to interact with engine from consol:\n";
        std::cout << "\thelp:\t\tprint this help\n";
        std::cout << "\tquit:\t\tclose program\n";
        std::cout << "\tposition:\tset FEN position\n";
        std::cout << "\tprint:\t\tprint current position\n";
        std::cout << "\treset:\t\treset board to starting position\n";
        std::cout << "\tmove <MOVE>:\tprovide MOVE in algebraic notation like e2e4!\n";
        std::cout << "\tundo:\t\tundo previous move\n";
        std::cout << "\tlist:\t\tlist all available moves from current position\n";
    }
}
