#ifndef VIXEN_UCI_HPP_INCLUDED
#define VIXEN_UCI_HPP_INCLUDED

#include <memory>

#include "defs.hpp"
#include "fixed_list.hpp"

namespace vixen
{

struct SearchInfo;

class Board;

class Move;

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

    void benchmark() const;

    static void LogUci(const SearchInfo &info, const int score, int depth, const FixedList<Move> &bestLine);

  private:
    std::unique_ptr<Board> board;

    std::unique_ptr<SearchInfo> info;

    void UpdateSearchInfo(std::istringstream &is, std::string &token);
};

} // namespace vixen

#endif // VIXEN_UCI_HPP_INCLUDED