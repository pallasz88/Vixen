#include "uci.h"
#include "board.h"
#include "move_generator.h"
#include "engine.h"

#include <iostream>
#include <sstream>

namespace Vixen::Uci
{

    void InitSearchInfo(const Board &board, std::istringstream &is, std::string &token, SearchInfo &info)
    {
        while (is >> token)
        {
            if (board.IsWhiteToMove() && token == "wtime")
            {
                is >> token;
                info.time[static_cast<int>(Colors::WHITE)] = std::stoi(token);
            }

            else if (!board.IsWhiteToMove() && token == "btime")
            {
                is >> token;
                info.time[static_cast<int>(Colors::BLACK)] = std::stoi(token);
            }

            if (board.IsWhiteToMove() && token == "winc")
            {
                is >> token;
                info.increment[static_cast<int>(Colors::WHITE)] = std::stoi(token);
            }

            else if (!board.IsWhiteToMove() && token == "binc")
            {
                is >> token;
                info.increment[static_cast<int>(Colors::BLACK)] = std::stoi(token);
            }

            if (token == "movestogo")
            {
                is >> token;
                info.movesToGo = std::stoi(token);
            }

            if (token == "movetime")
            {
                is >> token;
                info.moveTime = std::stoi(token);
            }

            if (token == "depth")
            {
                is >> token;
                info.maxDepth = std::stoi(token);
            }
        }
    }

    void loop(Board &board)
    {
        std::string line;
        while (std::getline(std::cin, line))
        {
            std::istringstream is(line);
            std::string token;
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
                SearchInfo info{};
                InitSearchInfo(board, is, token, info);
                const auto[score, encodedMove] = Search::IterativeDeepening(board, info);
                std::cout << "info score cp " << score << '\n';
                const auto from = encodedMove & 0x3FU;
                const auto to = (encodedMove >> 6U) & 0x3FU;
                std::cout << "bestmove " << SquareToNotation(from) << SquareToNotation(to) << '\n';
            }

            else if (token == "quit")
                return;
        }
    }
}