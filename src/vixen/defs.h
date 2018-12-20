#pragma once

#include <map>

#define VIXEN_API __declspec(dllexport)
#define STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TESTPOS1 "2rq1rk1/3bppbp/p5p1/1ppPP3/2n2B2/2P1Q1PP/P2N1PB1/R4RK1 w - - 1 18"
#define TESTPOS2 "rnbqkb1r/pp1ppppp/5n2/8/3p1B2/4P3/PPP2PPP/RN1QKBNR w KQkq - 0 4"
#define DEBUG

namespace Vixen
{

    typedef uint64_t BitBoard;

    typedef std::map<char, BitBoard> BitBoards;

    typedef std::array<int, 2> Direction;

    typedef std::array<Direction, 4> SliderDirections;

    typedef std::array<Direction, 8> AntiSliderDirections;

    typedef int Move;

    constexpr int MAX_SQUARE_INDEX = 63;

    constexpr int SQUARE_NUMBER = 64;

    constexpr int BISHOP_ATTACK_TABLE_SIZE = 0x1480;

    constexpr int ROOK_ATTACK_TABLE_SIZE = 0x19000;

    constexpr BitBoard EMPTY_BOARD = 0ULL;

    constexpr BitBoard BIT(int square)
    {
        return static_cast<BitBoard>(1) << square;
    }

    constexpr bool IsValidCoordinate(int file, int rank)
    {
        return file >= 0 && rank >= 0 && file < 8 && rank < 8;
    }

    template<class T, class U>
    inline bool IsBitSet(const T &x, const U &y)
    {
        return 0 != (x & y);
    }

    template<class T>
    inline void SetBit(T &x, unsigned y)
    {
        x |= 1ULL << y;
    }

    constexpr int PopCount(BitBoard b)
    {
        return __builtin_popcountll(b);
    }

    enum Ranks
    {
        RANK1 = 255ULL,
        RANK2 = 65280ULL,
        RANK7 = 71776119061217280ULL,
        RANK8 = 18374686479671623680ULL,
        RANK12 = 65535ULL,
        RANK18 = 18374686479671623935ULL,
        RANK78 = 18446462598732840960ULL
    };

    enum Files
    {
        FILEA = 9259542123273814144ULL,
        FILEB = 4629771061636907072ULL,
        FILEC = 2314885530818453536ULL,
        FILED = 1157442765409226768ULL,
        FILEE = 578721382704613384ULL,
        FILEF = 289360691352306692ULL,
        FILEG = 144680345676153346ULL,
        FILEH = 72340172838076673ULL
    };

    enum Squares
    {
        H1, G1, F1, E1, D1, C1, B1, A1,
        H2, G2, F2, E2, D2, C2, B2, A2,
        H3, G3, F3, E3, D3, C3, B3, A3,
        H4, G4, F4, E4, D4, C4, B4, A4,
        H5, G5, F5, E5, D5, C5, B5, A5,
        H6, G6, F6, E6, D6, C6, B6, A6,
        H7, G7, F7, E7, D7, C7, B7, A7,
        H8, G8, F8, E8, D8, C8, B8, A8
    };

}
