#include "uci.h"
#include "board.h"
#include "move_generator.h"

#include <iostream>
#include <sstream>
#include <random>

namespace Vixen::Uci
{

    void loop(Board &board)
    {
        std::random_device r;
        std::mt19937 e1(r());
        std::string line, token;
        while (std::getline(std::cin, line))
        {
            std::istringstream is(line);
            is >> token;
            
            if (token == "uci")
            {
                std::cout << "id name Vixen " << "0.0.5\n";
                std::cout << "id author Laszlo Paal\n\n";
                std::cout << "uciok\n";
            }

            else if (token == "ucinewgame")
                board.SetBoard(Constants::START_POSITION);

            else if (token == "isready")
                std::cout << "readyok\n";

            else if (token == "position") //position [fen  | startpos ]  moves  ....
            {
                std::string fen;

                is >> token;

                if (token == "startpos")
                {
                    fen = Constants::START_POSITION;
                    is >> token; // Consume "moves" token if any
                }
                else if (token == "fen")
                    while (is >> token && token != "moves")
                        fen += token + " ";
                else
                    return;

                board.SetBoard(fen);

                if (token == "moves")
                {
                    std::string move;
                    while (!is.eof())
                    {
                        is >> move;
                        board.MakeMove(move);
                    }
                }
            }

            else if (token == "go")
            {
                const auto generator = board.CreateGenerator<ALL_MOVE>();
                const auto moves = generator.GetLegalMoveList(board);
                std::uniform_int_distribution<unsigned> distribution(0, static_cast<unsigned>(moves.size() - 1));
                const unsigned move = moves[distribution(e1)];

                std::stringstream ss;
                ss << SquareToNotation(move & 0x3FU) << SquareToNotation((move & 0xFC0U) >> 6U);
                if ((move >> 12U) & PROMOTION)
                    ss << "nbrq"[static_cast<int>((move >> 12U) & 3U)];

                board.MakeMove(move);
                std::cout << "bestmove " << ss.str() << '\n';
            }

            else if (token == "quit")
                return;
        }
    }
}