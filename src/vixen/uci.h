#pragma once

#include "board.h"
#include <memory>

namespace Vixen
{
    struct SearchInfo;

    class VIXEN_API Uci
    {
    public:

        Uci();

        ~Uci();

        Uci(const Uci &) = delete;

        Uci &operator=(const Uci &) = delete;

        Uci(Uci &&) = delete;

        Uci &operator=(Uci &&) = delete;

        void loop();

        static void LogUci(const SearchInfo &info, const std::pair<int, Move> &result, int depth,
                           const std::vector<Move> &bestLine);

    private:

        std::unique_ptr<Board> board;

        std::unique_ptr<SearchInfo> info;

        void UpdateSearchInfo(std::istringstream &is, std::string &token);
    };


}
