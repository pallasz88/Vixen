#pragma once

#include <vector>
#include <string>
#include <map>

#define VIXEN_API __declspec(dllexport)
#define STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TESTPOS1 "2rq1rk1/3bppbp/p5p1/1ppPP3/2n2B2/2P1Q1PP/P2N1PB1/R4RK1 w - - 1 18"
#define TESTPOS2 "rnbqkb1r/pp1ppppp/5n2/8/3p1B2/4P3/PPP2PPP/RN1QKBNR w KQkq - 0 4"
#define MAX_SHIFT_NUM 63
#define DEBUG

namespace Vixen
{

    typedef uint64_t BitBoard;

    typedef std::map<char, BitBoard> BitBoards;

    inline unsigned long long BIT(unsigned x)
    {
        unsigned long long shiftMe = 1;
        return shiftMe << x;
    }

    template<class T, class U>
    inline bool IsBitSet(const T &x, const U &y)
    {
        return 0 != (x & y);
    }

    enum Ranks
    {
        RANK1 = 255U,
        RANK2 = 65280U,
        RANK7 = 71776119061217280U,
        RANK8 = 18374686479671623680U,
        RANK12 = 65535U,
        RANK18 = 18374686479671623935U,
        RANK78 = 18446462598732840960U
    };

    enum Files
    {
        FILEA = 9259542123273814144U,
        FILEB = 4629771061636907072U,
        FILEG = 144680345676153346U,
        FILEH = 72340172838076673U,
        FILEAB = 13889313184910721216U,
        FILEGH = 217020518514230019U
    };

    typedef enum
    {
        H1, G1, F1, E1, D1, C1, B1, A1,
        H2, G2, F2, E2, D2, C2, B2, A2,
        H3, G3, F3, E3, D3, C3, B3, A3,
        H4, G4, F4, E4, D4, C4, B4, A4,
        H5, G5, F5, E5, D5, C5, B5, A5,
        H6, G6, F6, E6, D6, C6, B6, A6,
        H7, G7, F7, E7, D7, C7, B7, A7,
        H8, G8, F8, E8, D8, C8, B8, A8
    } Square;

    const std::vector<std::string> squares =
            {
                    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",
                    "h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",
                    "h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",
                    "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",
                    "h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",
                    "h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
                    "h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",
                    "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8"
            };

}
