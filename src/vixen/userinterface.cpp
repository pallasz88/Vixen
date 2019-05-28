#include <iostream>
#include <regex>
#include "userinterface.h"
#include "move_generator.h"
#include "board.h"

namespace Vixen
{
    void UserInterface::WaitUserInput(Vixen::Board &board)
    {
        while (true)
        {
            std::string command;

            std::getline(std::cin, command);

            if (command == "quit")
                return;

            else if (command == "print")
                board.PrintBoard();

            else if (command == "reset")
                board.SetBoard(START_POSITION);

            else if (command.substr(0, 4) == "move")
                try
                {
                    MakeMove(command.substr(5), board);
                } catch (std::runtime_error &error)
                {
                    std::cerr << "ERROR HAPPENED: " << error.what() << "\n";
                }

            else if (command == "undo")
                try
                {
                    TakeBackMove(board);
                } catch (std::runtime_error &error)
                {
                    std::cerr << "ERROR HAPPENED: " << error.what() << "\n";
                }

            else
                std::cerr << "UNKNOWN COMMAND: " << command << "\n";
        }
    }

    void UserInterface::MakeMove(std::string &&move, Vixen::Board &board)
    {
        bool isLegal = false;
        std::cout << "Your move is: " << move << "\n";
        std::regex moveRegex{"[a-h][1-8][a-h][1-8]"};
        if (!std::regex_match(move, moveRegex))
            throw std::runtime_error("INVALID MOVE FORMAT. Use for example e2e4.");

        int from = NotationToSquare(move.substr(0, 2));
        int to = NotationToSquare(move.substr(2));
        int decodedMove = to << 6 | from;
        std::vector<Move> moves = MoveGenerator::GetAllMoves(board);

        for(const auto& pseudoMove : moves)
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

    void UserInterface::TakeBackMove(Board &board)
    {
        board.TakeBack();
    }
}
