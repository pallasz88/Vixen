#include <iostream>
#include <regex>
#include "userinterface.h"
#include "move_generator.h"
#include "board.h"
#include "timer.h"

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

            else if (command.substr(0, 5) == "perft")
            {
                Timer<std::chrono::milliseconds> timer("Perft test");
                std::cout << "Visited nodes: " << Test::PerftTest(stoi(command.substr(6)), board) << "\n";
            }

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

        MoveGenerator generator = board.CreateGenerator();
        std::array<Move, 300> moves = generator.GetMoveList();
        auto moveListSize = generator.GetListSize();

        for (size_t i = 0; i < moveListSize; ++i)
            if ((moves[i] & 0xFFF) == decodedMove)
            {
                if (!board.MakeMove(moves[i]))
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
        MoveGenerator generator = board.CreateGenerator();
        std::array<Move, 300> movesList = generator.GetMoveList();
        size_t moveListSize = generator.GetListSize();
        for (size_t i = 0; i < moveListSize; ++i)
        {
            if (board.MakeMove(movesList[i]))
            {
                std::cout << SquareToNotation(movesList[i] & 0x3F)
                          << SquareToNotation((movesList[i] & 0xFC0) >> 6);
                if ((movesList[i] >> 12) & PROMOTION)
                    std::cout << "nbrq"[static_cast<int>((movesList[i] >> 12) & 3)];

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
