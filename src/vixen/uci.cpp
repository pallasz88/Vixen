#include "uci.hpp"

#include <iostream>
#include <sstream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"

namespace vixen
{

Uci::Uci() : board(std::make_unique<Board>()), info(std::make_unique<SearchInfo>())
{
}

Uci::~Uci()
{
    board.reset();
    info.reset();
}

void Uci::LogUci(const SearchInfo &info, const std::pair<int, Move> &result, int depth, const FixedList<Move> &bestLine)
{
    if (result.first > Search::MATE - info.maxDepth)
        std::cout << "info score mate " << (Search::MATE - result.first) / 2;

    else if (result.first < -Search::MATE + info.maxDepth)
        std::cout << "info score mate " << (-Search::MATE - result.first) / 2;

    else
        std::cout << "info score cp " << result.first;

    std::cout << " depth " << depth << " nodes " << info.nodesCount << " pv ";

    for (const auto &move : bestLine)
        std::cout << move << " ";

    std::cout << std::endl;
}

void Uci::UpdateSearchInfo(std::istringstream &is, std::string &token)
{
    info->nodesCount = 0;

    while (is >> token)
    {
        if (board->IsWhiteToMove() && token == "wtime")
        {
            is >> token;
            info->isTimeSet = true;
            info->time[static_cast<int>(Colors::WHITE)] = std::stoi(token);
        }

        else if (!board->IsWhiteToMove() && token == "btime")
        {
            is >> token;
            info->isTimeSet = true;
            info->time[static_cast<int>(Colors::BLACK)] = std::stoi(token);
        }

        if (board->IsWhiteToMove() && token == "winc")
        {
            is >> token;
            info->isTimeSet = true;
            info->increment[static_cast<int>(Colors::WHITE)] = std::stoi(token);
        }

        else if (!board->IsWhiteToMove() && token == "binc")
        {
            is >> token;
            info->isTimeSet = true;
            info->increment[static_cast<int>(Colors::BLACK)] = std::stoi(token);
        }

        if (token == "movestogo")
        {
            is >> token;
            info->movesToGo = std::stoi(token);
        }

        if (token == "movetime")
        {
            is >> token;
            info->isTimeSet = true;
            info->moveTime = std::stoi(token);
        }

        if (token == "depth")
        {
            is >> token;
            info->isTimeSet = false;
            info->maxDepth = std::stoi(token);
        }
    }
}

void Uci::loop()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        std::istringstream is(line);
        std::string token;
        is >> token;

        if (token == "uci")
        {
            std::cout << "id name Vixen "
                      << "0.0.5\n";
            std::cout << "id author Laszlo Paal\n\n";
            std::cout << "uciok\n";
        }

        else if (token == "ucinewgame")
            board->SetBoard(Constants::START_POSITION);

        else if (token == "isready")
            std::cout << "readyok\n";

        else if (token == "position") // position [fen  | startpos ]  moves  ....
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

            board->SetBoard(fen);

            if (token == "moves")
            {
                std::string move;
                while (!is.eof())
                {
                    is >> move;
                    static_cast<void>(board->MakeMove(move));
                }
            }
        }

        else if (token == "go")
        {
            UpdateSearchInfo(is, token);
            std::thread t(&Search::IterativeDeepening, std::ref(*board), std::ref(*info));
            t.detach();
        }

        else if (token == "quit")
            return;

        else if (token == "stop")
        {
            info->stopped = true;
        }
    }
}

void Uci::benchmark() const
{
    board->SetBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    info->maxDepth = 12;
    Search::IterativeDeepening(*board, *info);
}

} // namespace vixen