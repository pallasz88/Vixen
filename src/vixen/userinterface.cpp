#include "userinterface.hpp"

#include <iostream>
#include <regex>

#include "board.hpp"
#include "move_generator.hpp"
#include "timer.hpp"

namespace vixen::UserInterface
{
void WaitUserInput(vixen::Board &board)
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
            board.SetBoard(command.substr(9));

        else if (command == "print")
            board.PrintBoard();

        else if (command == "reset")
            board.SetBoard(Constants::START_POSITION);

        else if (command.substr(0, 5) == "perft")
        {
            Timer<std::chrono::milliseconds> timer("Perft test");
            std::cout << Test::PerftTest(stoi(command.substr(6)), board) << "\n";
        }

        else if (command.substr(0, 4) == "move")
            try
            {
                UserInterface::MakeMove(command.substr(5), board);
            }
            catch (std::runtime_error &error)
            {
                std::cerr << error.what() << "\n";
            }
            catch (std::out_of_range &error)
            {
                std::cerr << "INVALID MOVE FORMAT. Use for example e2e4.\n";
            }

        else if (command == "undo")
            try
            {
                TakeBackMove(board);
            }
            catch (std::runtime_error &error)
            {
                std::cerr << "ERROR HAPPENED: " << error.what() << "\n";
            }

        else if (command == "list")
            PrintMoveList(board);

        else
            std::cerr << "UNKNOWN COMMAND: " << command << "\n";
    }
}

void MakeMove(std::string &&move, vixen::Board &board)
{
    std::cout << "Your move is: " << move << "\n";

    if (!board.MakeMove(move))
    {
        std::cerr << "Illegal move!\n";
        return;
    }

    board.PrintBoard();
}

void TakeBackMove(Board &board)
{
    board.TakeBack();
}

void PrintMoveList(Board &board)
{
    auto moveList = board.GetMoveList<static_cast<uint8_t>(MoveTypes::ALL_MOVE)>();
    for (const auto move : moveList)
    {
        if (board.MakeMove(move))
        {
            std::cout << move;
            if ((move.GetMoveType()) & static_cast<uint8_t>(MoveTypes::PROMOTION))
                std::cout << "nbrq"[static_cast<int>((move.GetMoveType()) & 3U)];

            std::cout << ", ";
            board.TakeBack();
        }
    }
    std::cout << "\n";
}

void PrintHelp()
{
    std::cout << "Welcome to Vixen C++ chess engine!\n";
    std::cout << "\nUse the below listed commands to interact with engine from "
                 "consol:\n";
    std::cout << "\thelp:\t\tprint this help\n";
    std::cout << "\tquit:\t\tclose program\n";
    std::cout << "\tposition:\tset FEN position\n";
    std::cout << "\tprint:\t\tprint current position\n";
    std::cout << "\treset:\t\treset board to starting position\n";
    std::cout << "\tmove:\t\tprovide your move in algebraic notation like e2e4!\n";
    std::cout << "\tundo:\t\tundo previous move\n";
    std::cout << "\tlist:\t\tlist all available moves from current position\n";
    std::cout << "\tperft:\t\tcomputer generates legal moves from current "
                 "position to given depth\n";
}
} // namespace Vixen::UserInterface
